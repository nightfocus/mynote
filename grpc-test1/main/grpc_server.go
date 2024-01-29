package main

// go build grpc_server.go multi_interceptor.go

import (
	"context"
	"flag"
	"fmt"
	"github.com/grpc-ecosystem/grpc-gateway/v2/runtime"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/grpc/status"
	"log"
	"net"
	"net/http"
	"net/http/pprof"
	"time"

	"google.golang.org/grpc"
	pb "grpc-test1/helloworld"
)

const (
	port        = ":50051"
	pprofPrefix = "/debug/pprof"
)

// greeterServer 定义一个结构体用于实现 .proto文件中定义的方法
// 新版本 gRPC 要求必须嵌入 pb.UnimplementedGreeterServer 结构体
type greeterServer struct {
	pb.UnimplementedGreeteraaaServer
}

var restful = flag.Int("restful", 8080, "the port to restful serve on")

// SayHello 简单实现一下.proto文件中定义的 SayHello 方法
func (g *greeterServer) SayHello(ctx context.Context, in *pb.HelloRequest) (*pb.HelloReply, error) {
	log.Printf("Received msg: %+v", *in)
	// return nil, errors.New("func is error.")
	return &pb.HelloReply{Message: `{ "msggg": "Hello -------- ", "vvv" : "in.GetName()" }`}, nil
}

// 服务端的一元拦截器
func unaryInterceptor(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (interface{}, error) {
	fmt.Printf("In unaryInterceptor()\n")
	start := time.Now()
	if start.Second()%300 == 0 {
		// 这里候，http 调用者 会收到 http 500 的响应，grpc有自己的错误码规则。
		// 参考：https://github.com/googleapis/googleapis/blob/master/google/rpc/code.proto
		// https://zhuanlan.zhihu.com/p/414316324
		// {
		//    "code": 2,
		//    "message": "can't accpet.",
		//    "details": []
		//}
		// grpc client RPC会返回err： code = Unknown desc = can't accpet.
		fmt.Printf("failed per 3 times.\n")
		return nil, status.Errorf(codes.InvalidArgument, "can't accpet.")
	}

	m, err := handler(ctx, req)
	end := time.Now()
	// 记录请求参数 耗时 错误信息等数据
	fmt.Printf("RPC: %s,req:%v start time: %s, end time: %s, err: %v\n",
		info.FullMethod, req, start.Format(time.RFC3339), end.Format(time.RFC3339), err)
	return m, err
}

// 服务端的一元拦截器
func unaryInterceptor2(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (interface{}, error) {
	fmt.Printf("In unaryInterceptor2()\n")
	return handler(ctx, req)
}

func main() {
	listen, err := net.Listen("tcp", port)
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
	}

	// 注册多个中间件
	interceptorChina := NewMultiUnaryServerInterceptor(
		unaryInterceptor,
		unaryInterceptor2,
	)
	s := grpc.NewServer(grpc.UnaryInterceptor(interceptorChina))

	// 将服务描述(server)及其具体实现(greeterServer)注册到 gRPC 中去.
	// 内部使用的是一个 map 结构存储，类似 HTTP server。
	pb.RegisterGreeteraaaServer(s, &greeterServer{})
	log.Println("Serving gRPC on 0.0.0.0" + port)
	go func() {
		if err := s.Serve(listen); err != nil {
			log.Fatalf("failed to serve: %v", err)
		}
	}()

	// 2. 启动 grpc HTTP 服务
	// Create a client connection to the gRPC server we just started
	// This is where the gRPC-Gateway proxies the requests
	conn, err := grpc.Dial(
		"localhost:50051",
		grpc.WithTransportCredentials(insecure.NewCredentials()),
	)
	if err != nil {
		log.Fatalln("Failed to dial server:", err)
	}

	gwmux := runtime.NewServeMux()
	// Register Greeter
	err = pb.RegisterGreeteraaaHandler(context.Background(), gwmux, conn)
	if err != nil {
		log.Fatalln("Failed to register gateway:", err)
	}

	//新建mux，它是http的请求复用器
	mux := http.NewServeMux()
	//注册gwmux 把gRPC服务转成HTTP服务，让gRPC同时支持HTTP
	mux.Handle("/", gwmux)

	//pprof
	mux.Handle(pprofPrefix+"/", http.HandlerFunc(pprof.Index))
	mux.Handle(pprofPrefix+"/profile", http.HandlerFunc(pprof.Profile))
	mux.Handle(pprofPrefix+"/symbol", http.HandlerFunc(pprof.Symbol))
	mux.Handle(pprofPrefix+"/cmdline", http.HandlerFunc(pprof.Cmdline))
	mux.Handle(pprofPrefix+"/trace", http.HandlerFunc(pprof.Trace))
	mux.Handle(pprofPrefix+"/heap", pprof.Handler("heap"))
	mux.Handle(pprofPrefix+"/goroutine", pprof.Handler("goroutine"))
	mux.Handle(pprofPrefix+"/threadcreate", pprof.Handler("threadcreate"))
	mux.Handle(pprofPrefix+"/block", pprof.Handler("block"))

	gwServer := &http.Server{
		Addr:    fmt.Sprintf(":%d", *restful),
		Handler: mux,
	}

	// 开3个协程跑着，是为了看pprof情况
	// 执行命令，采集30秒后启动浏览器界面
	// go tool pprof -http=:7771 http://localhost:8080/debug/pprof/profile?seconds=30
	go Fcunc(1)
	go Fcunc(10)
	go Fcunc(100)

	log.Println("Serving gRPC-Gateway http on http://0.0.0.0" + fmt.Sprintf(":%d", *restful))
	log.Fatalln(gwServer.ListenAndServe())
}

func Fcunc(n time.Duration) {
	for {
		time.Sleep(n * time.Microsecond)
	}
	log.Println("end.")
}
