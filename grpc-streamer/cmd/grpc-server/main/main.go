package main

import (
	"io"
	"log"
	"net"
	"time"

	"google.golang.org/grpc"
	pb "grpc-streamer/cmd/proto/stream_demo" // 确保这个路径指向你的 .proto 文件生成的包
)

type server struct {
	pb.UnimplementedStreamServiceServer
}

func (s *server) StreamDemo(stream pb.StreamService_StreamDemoServer) error {
	for {
		req, err := stream.Recv()
		if err == io.EOF {
			// 客户端关闭了连接
			return nil
		}
		if err != nil {
			log.Printf("Error receiving: %v", err)
			break
		}
		log.Printf("Received: %s", req.Message)

		// 持续发送响应多次
		session := int64(time.Now().Nanosecond())
		go func() {
			for i := int32(0); i < 3; i++ {
				resp := pb.Response{
					Message: req.Message,
					Session: session,
					Cnt:     i,
				}
				// 发送响应的数据流
				if err := stream.Send(&resp); err != nil {
					log.Printf("Error sending: %v", err)
				}
			}
		}()
	}

	return nil
}

var serverPort string = ":50051"

func main() {
	listener, err := net.Listen("tcp", serverPort)
	if err != nil {
		log.Fatalf("failed to listen: %v\n", err)
	}

	log.Printf("Listen %s\n", serverPort)
	s := grpc.NewServer()
	pb.RegisterStreamServiceServer(s, &server{})
	log.Printf("server listening at %v\n", listener.Addr())
	if err := s.Serve(listener); err != nil {
		log.Fatalf("failed to serve: %v\n", err)
	}
}
