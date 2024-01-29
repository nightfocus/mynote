package main

import (
	"context"
	"io"
	"log"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	pb "grpc-streamer/cmd/proto/stream_demo"
)

func main() {
	conn, err := grpc.Dial(":50051", grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	defer conn.Close()
	c := pb.NewStreamServiceClient(conn)

	stream, err := c.StreamDemo(context.Background())
	if err != nil {
		log.Fatalf("could not create stream: %v", err)
	}

	go func() {
		for {
			recvBuff, err := stream.Recv()
			if err == io.EOF {
				// 服务器关闭了连接
				return
			}
			if err != nil {
				log.Fatalf("failed to receive a message: %v", err)
			}
			log.Printf("Got message from server: %d,%d, %s", recvBuff.Session, recvBuff.GetCnt(), recvBuff.GetMessage())
			// log.Printf("Got message from server: %s", recvBuff.GetMessage())
		}
	}()

	for {
		message := "Hello " + "World " + time.Now().String()
		if err := stream.Send(&pb.Request{Message: message}); err != nil {
			log.Fatalf("failed to send a message: %v", err)
		}
		log.Printf("Sent message to server: %s", message)
		time.Sleep(999 * time.Millisecond) // 999 ms
	}
}
