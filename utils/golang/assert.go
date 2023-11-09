package golang

import (
	"fmt"
	"reflect"
)

func AssertEqual(a, b any) {
	if !reflect.DeepEqual(a, b) {
		panic(fmt.Sprintf("%v != %b", a, b))
	} else {
		fmt.Printf("%v assert equal pass\n", a)
	}
}
