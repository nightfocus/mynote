package main

import (
	"fmt"
	"runtime"
	// "syscall"
)

/*
type File struct {
	d int
}

func openFile(path string) *File {
	d, err := syscall.Open(path, syscall.O_RDONLY, 0)
	if err != nil {
		panic(err)
	}

	p := &File{d}
	runtime.SetFinalizer(p, func(p *File) {
		fmt.Println("GC")
		syscall.Close(p.d)
	})
	return p
}

func doSomeAllocation() {
	var a *int

	// memory increase to force the GC
	for i := 0; i < 10000000; i++ {
		i := 1
		a = &i
	}

	_ = a
}

func readFile(fd int) string {
	runtime.GC()
	fmt.Println(1)
	runtime.GC()
	fmt.Println(2)
	runtime.GC()
	fmt.Println(3)
	//runtime.GC()
	//runtime.GC()
	//doSomeAllocation()

	var buf [1000]byte
	_, err := syscall.Read(fd, buf[:])
	if err != nil {
		panic(err)
	}
	//fmt.Println(n)
	//fmt.Println(buf)
	return string(buf[:])
}



func (f *File) keepAlive() {

}
*/
type Core struct {
}

func (c *Core) Get() string {
	return "c.Get()"
}

type Wrap struct {
	c *Core
}

func (c *Core) DoSomeThing() {
	//runtime.GC()
	//runtime.GC()
	//runtime.GC()
	//runtime.GC()
	runtime.GC()
	runtime.GC()

	fmt.Println("core " + c.Get())
	// _ = c.Get()
}

func (w *Wrap) DoSomeThing() {
	// defer runtime.KeepAlive(w)
	w.c.DoSomeThing()
}

func NewWrap() *Wrap {
	w := new(Wrap)
	runtime.SetFinalizer(w, func(w *Wrap) {
		fmt.Println("wrap GC")
	})
	return w
}

func NewCore() *Core {
	c := &Core{}
	runtime.SetFinalizer(c, func(w *Core) {
		fmt.Println("core GC")
	})
	return c
}

func A() *Wrap {
	w := NewWrap()
	w.c = NewCore()
	return w
}

func main() {

	// p := openFile("./main.go.bak")
	//defer p.keepAlive()
	// _ = readFile(p.d)
	//fmt.Println(content)
	// runtime.KeepAlive(p)

	w := A()
	w.DoSomeThing()
}

/*


/*
#include <stdlib.h>
typedef int (*qsort_cmp_func_t) (const void* a, const void* b);
extern int go_qsort_compare(void* a, void* b);

import "C"
import "unsafe"

import (
	"fmt"
	"sync"
	"time"
)

func quickSort(arr []int) []int {
	if len(arr) < 2 {
		return arr
	}
	pivot := arr[0]
	var left, right []int
	for _, v := range arr[1:] {
		if v < pivot {
			left = append(left, v)
		} else {
			right = append(right, v)
		}
	}
	left = quickSort(left)
	right = quickSort(right)
	return append(append(left, pivot), right...)
}

//export go_qsort_compare
func go_qsort_compare(a, b unsafe.Pointer) C.int {
	pa, pb := (*C.int)(a), (*C.int)(b)
	return C.int(*pa - *pb)
}

// 我们模拟C包中的类型通过Go语言类型代替，在内部调用C函数qsort()时重新转型为C函数需要的类型。
type CompareFunc C.qsort_cmp_func_t

func MySort(base unsafe.Pointer, num, size int, cmp CompareFunc) {
	C.qsort(base, C.size_t(num), C.size_t(size), C.qsort_cmp_func_t(cmp))
}

func main() {
	// 快排
	arr := []int{5, 3, 8, 4, 2}
	fmt.Println(quickSort(arr))

	// 用cgo方式调用C语言的qsort做排序
	values := []int32{5, 2, 4, 9, 10, 3, 8, 1}
	MySort(unsafe.Pointer(&values[0]), len(values), int(unsafe.Sizeof(values[0])), CompareFunc(C.go_qsort_compare))
	fmt.Println(values)

	var wg sync.WaitGroup
	var count int
	var ch = make(chan bool, 1)

	for i := 0; i <= 1000; i++ {
		wg.Add(1)
		go func() {
			ch <- true
			count++
			time.Sleep(1 * time.Millisecond)
			count--
			<-ch
			wg.Done()
		}()
	}
	wg.Wait()
	fmt.Println(count)
}

/*
import (
	"github.com/tidwall/pretty"
	"net/http"

	"github.com/gin-gonic/gin"
)

func main() {

	router := gin.Default()

	ssjson := "{\"name\":  {\"first\":\"Tom\",\"last\":\"Anderson\"},  \"age\":37,\n\"children\": [\"Sara\",\"Alex\",\"Jack\"],\n\"fav.movie\": \"Deer Hunter\", \"friends\": [\n    {\"first\": \"Janet\", \"last\": \"Murphy\", \"age\": 44}\n  ]}"
	s := pretty.Pretty([]byte(ssjson))
	router.GET("/", func(c *gin.Context) {
		c.String( http.StatusOK, string(s))
	})

	router.Run("127.0.0.1:8080")
}
*/

/*package main

import (
	"context"
	"crypto/ecdsa"
	"fmt"
	"log"
	"math/big"

	ethereum "github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/crypto"
	"github.com/ethereum/go-ethereum/ethclient"
	"golang.org/x/crypto/sha3"
)

func main() {
	client, err := ethclient.Dial("https://rinkeby.infura.io")
	if err != nil {
		log.Fatal(err)
	}

	privateKey, err := crypto.HexToECDSA("fad9c8855b740a0b7ed4c221dbad0f33a83a49cad6b3fe8d5817ac83d38b6a19")
	if err != nil {
		log.Fatal(err)
	}

	publicKey := privateKey.Public()
	publicKeyECDSA, ok := publicKey.(*ecdsa.PublicKey)
	if !ok {
		log.Fatal("cannot assert type: publicKey is not of type *ecdsa.PublicKey")
	}

	fromAddress := crypto.PubkeyToAddress(*publicKeyECDSA)
	nonce, err := client.PendingNonceAt(context.Background(), fromAddress)
	if err != nil {
		log.Fatal(err)
	}

	value := big.NewInt(0) // in wei (0 eth)
	gasPrice, err := client.SuggestGasPrice(context.Background())
	if err != nil {
		log.Fatal(err)
	}

	toAddress := common.HexToAddress("0x4592d8f8d7b001e72cb26a73e4fa1806a51ac79d")
	tokenAddress := common.HexToAddress("0x28b149020d2152179873ec60bed6bf7cd705775d")

	transferFnSignature := []byte("transfer(address,uint256)")
	hash := sha3.NewLegacyKeccak256()
	hash.Write(transferFnSignature)
	methodID := hash.Sum(nil)[:4]
	fmt.Println(hexutil.Encode(methodID)) // 0xa9059cbb

	paddedAddress := common.LeftPadBytes(toAddress.Bytes(), 32)
	fmt.Println(hexutil.Encode(paddedAddress)) // 0x0000000000000000000000004592d8f8d7b001e72cb26a73e4fa1806a51ac79d

	amount := new(big.Int)
	amount.SetString("1000000000000000000000", 10) // sets the value to 1000 tokens, in the token denomination

	paddedAmount := common.LeftPadBytes(amount.Bytes(), 32)
	fmt.Println(hexutil.Encode(paddedAmount)) // 0x00000000000000000000000000000000000000000000003635c9adc5dea00000

	var data []byte
	data = append(data, methodID...)
	data = append(data, paddedAddress...)
	data = append(data, paddedAmount...)

	gasLimit, err := client.EstimateGas(context.Background(), ethereum.CallMsg{
		To:   &tokenAddress,
		Data: data,
	})
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(gasLimit) // 23256

	tx := types.NewTransaction(nonce, tokenAddress, value, gasLimit, gasPrice, data)

	chainID, err := client.NetworkID(context.Background())
	if err != nil {
		log.Fatal(err)
	}

	signedTx, err := types.SignTx(tx, types.NewEIP155Signer(chainID), privateKey)
	if err != nil {
		log.Fatal(err)
	}

	err = client.SendTransaction(context.Background(), signedTx)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("tx sent: %s", signedTx.Hash().Hex()) // tx sent: 0xa56316b637a94c4cc0331c73ef26389d6c097506d581073f927275e7a6ece0bc
}
*/
