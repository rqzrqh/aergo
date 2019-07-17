package system

import (
	"container/list"
	"encoding/binary"
	"math/big"

	"github.com/aergoio/aergo/state"
	"github.com/aergoio/aergo/types"
)

const (
	vprMax        = 50000
	vprBucketsMax = 1024
)

var (
	vprKeyPrefix = []byte("VotingPowerBucket")
	rank         = newVpr()
)

type votingPower struct {
	addr  types.AccountID
	power *big.Int
}

func newVotingPower(addr types.AccountID, pow *big.Int) *votingPower {
	return &votingPower{addr: addr, power: pow}
}

func (vp *votingPower) set(pow *big.Int) {
	vp.power = pow
}

func (vp *votingPower) cmp(pow *big.Int) int {
	return vp.power.Cmp(pow)
}

type vprBucket struct {
	buckets map[uint16]*list.List
	max     uint16
	cmp     func(pow *big.Int) func(v *votingPower) int
}

func newVprBucket(max uint16) *vprBucket {
	return &vprBucket{
		max:     max,
		buckets: make(map[uint16]*list.List, vprBucketsMax),
		cmp: func(pow *big.Int) func(v *votingPower) int {
			return func(v *votingPower) int {
				return v.cmp(pow)
			}
		},
	}
}

func (b *vprBucket) update(addr types.AccountID, pow *big.Int) {
	var (
		idx   = getBucketIdx(addr)
		bu    *list.List
		exist bool
	)

	if bu, exist = b.buckets[idx]; !exist {
		bu = list.New()
		b.buckets[idx] = bu
	}

	v := remove(bu, addr)
	if v != nil {
		v.set(pow)
	} else {
		v = newVotingPower(addr, pow)
	}

	if m := findInsPos(bu, b.cmp(pow)); m != nil {
		bu.InsertBefore(v, m)
	} else {
		bu.PushBack(v)
	}
}

func remove(bu *list.List, addr types.AccountID) *votingPower {
	for e := bu.Front(); e != nil; e = e.Next() {
		if v := e.Value.(*votingPower); addr == v.addr {
			return bu.Remove(e).(*votingPower)
		}
	}
	return nil
}

func findInsPos(bu *list.List, fn func(v *votingPower) int) *list.Element {
	for e := bu.Front(); e != nil; e = e.Next() {
		v := e.Value.(*votingPower)
		ind := fn(v)
		if ind < 0 || ind == 0 {
			return e
		}
	}

	return nil
}

func getBucketIdx(addr types.AccountID) uint16 {
	return binary.LittleEndian.Uint16(addr[:2]) & 0x3FF

}

func (b *vprBucket) getBucket(addr types.AccountID) *list.List {
	if b, exist := b.buckets[getBucketIdx(addr)]; exist {
		return b
	}
	return nil
}

// Voters Power Ranking (VPR)
type vpr struct {
	votingPower map[types.AccountID]*big.Int
	changes     map[types.AccountID]*big.Int
	table       *vprBucket
}

func newVpr() *vpr {
	return &vpr{
		votingPower: make(map[types.AccountID]*big.Int, vprMax),
		changes:     make(map[types.AccountID]*big.Int, vprMax),
		table:       newVprBucket(vprBucketsMax),
	}
}

func (v *vpr) votingPowerOf(address types.AccountID) *big.Int {
	return v.votingPower[address]
}

func (v *vpr) update(addr types.AccountID, fn func(lhs *big.Int)) {
	if _, exist := v.votingPower[addr]; !exist {
		v.votingPower[addr] = new(big.Int)
	}

	if _, exist := v.changes[addr]; !exist {
		v.changes[addr] = new(big.Int).Set(v.votingPower[addr])
	}
	ch := v.changes[addr]

	fn(ch)
}

func (v *vpr) set(addr types.AccountID, power *big.Int) {
	v.update(addr,
		func(lhs *big.Int) {
			lhs.Set(power)
		},
	)
}

func (v *vpr) add(addr types.AccountID, power *big.Int) {
	v.update(addr,
		func(lhs *big.Int) {
			lhs.Add(lhs, power)
		},
	)
}

func (v *vpr) sub(addr types.AccountID, power *big.Int) {
	v.update(addr,
		func(lhs *big.Int) {
			lhs.Sub(lhs, power)
		},
	)
}

func (v *vpr) apply(s *state.ContractState) {
	for key, pow := range v.changes {
		if curPow := v.votingPower[key]; curPow.Cmp(pow) != 0 {
			v.votingPower[key] = pow
			v.table.update(key, pow)

			delete(v.changes, key)
			if s != nil {
				s.SetRawKV(vprKey(key[:]), pow.Bytes())
			}
		}
	}
}

func (v *vpr) Bingo(seed []byte) {
}

func vprKey(key []byte) []byte {
	var vk []byte = vprKeyPrefix
	return append(vk, key...)
}
