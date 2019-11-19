package fee

import (
	"math"
	"math/big"
)

const (
	txGasSize = 100000
	payloadGasSize = 5
)

func TxGas(payloadSize int) uint64 {
	if IsZeroFee() {
		return 0
	}
	size := paymentDataSize(int64(payloadSize))
	if size > payloadMaxSize {
		size = payloadMaxSize
	}
	txGas := uint64(txGasSize)
	payloadGas := uint64(size) * uint64(payloadGasSize)
	return txGas + payloadGas
}

func MaxGasLimit(balance, gasPrice *big.Int) uint64 {
	gasLimit := uint64(math.MaxUint64)
	n := balance.Div(balance, gasPrice)
	if n.IsUint64() {
		gasLimit = n.Uint64()
	}
	return gasLimit
}
