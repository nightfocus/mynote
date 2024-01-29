package main

import (
	"context"
	_struct "github.com/golang/protobuf/ptypes/struct"
	"google.golang.org/grpc/credentials/insecure"
	"log"
	"os"
	"time"

	"google.golang.org/grpc"
	pb "grpc-test1/helloworld"
)

const (
	address     = "localhost:50051"
	defaultName = "worldx"
)

func main() {
	conn, err := grpc.Dial(address, grpc.WithTransportCredentials(insecure.NewCredentials()), grpc.WithBlock())
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	defer conn.Close()
	c := pb.NewGreeteraaaClient(conn)

	// 通过命令行参数指定 name
	name := defaultName
	if len(os.Args) > 1 {
		name = os.Args[1]
	}

	// arrayst := make([]string, 0)
	// arrayst[0] = "1"
	vvv := &_struct.ListValue{}

	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()
	r, err := c.SayHello(ctx, &pb.HelloRequest{Name: name, Arrays: vvv})
	if err != nil {
		log.Fatalf("could not greet: %v", err)
	}
	log.Printf("Greeting: %s", r.GetMessage())
}
