package sbp

import (
	"time"

	"github.com/aergoio/aergo-lib/log"
	"github.com/aergoio/aergo/config"
	"github.com/aergoio/aergo/consensus"
	"github.com/aergoio/aergo/consensus/chain"
	"github.com/aergoio/aergo/pkg/component"
	"github.com/aergoio/aergo/state"
	"github.com/aergoio/aergo/types"
)

const (
	slotQueueMax = 100
)

var logger *log.Logger

func init() {
	logger = log.NewLogger("sbp")
}

// SimpleBlockFactory implments a simple block factory which generate block each cfg.Consensus.BlockInterval.
//
// This can be used for testing purpose.
type SimpleBlockFactory struct {
	*component.ComponentHub
	jobQueue         chan interface{}
	blockInterval    time.Duration
	maxBlockBodySize uint32
	txOp             chain.TxOp
	quit             chan interface{}
}

// New returns a SimpleBlockFactory.
func New(cfg *config.Config, hub *component.ComponentHub) (*SimpleBlockFactory, error) {
	consensus.InitBlockInterval(cfg.Consensus.BlockInterval)

	s := &SimpleBlockFactory{
		ComponentHub:     hub,
		jobQueue:         make(chan interface{}, slotQueueMax),
		blockInterval:    consensus.BlockInterval,
		maxBlockBodySize: chain.MaxBlockBodySize(),
		quit:             make(chan interface{}),
	}

	s.txOp = chain.NewCompTxOp(
		chain.TxOpFn(func(txIn *types.Tx) (*types.BlockState, error) {
			select {
			case <-s.quit:
				return nil, chain.ErrQuit
			default:
				return nil, nil
			}
		}),
	)

	return s, nil
}

// Ticker returns a time.Ticker for the main consensus loop.
func (s *SimpleBlockFactory) Ticker() *time.Ticker {
	return time.NewTicker(s.blockInterval)
}

// QueueJob send a block triggering information to jq.
func (s *SimpleBlockFactory) QueueJob(now time.Time, jq chan<- interface{}) {
	if b := chain.GetBestBlock(s); b != nil {
		jq <- b
	}
}

// SetStateDB do nothing in the simple block factory, which do not execute
// transactions at all.
func (s *SimpleBlockFactory) SetStateDB(sdb *state.ChainStateDB) {
}

// IsTransactionValid checks the onsensus level validity of a transaction
func (s *SimpleBlockFactory) IsTransactionValid(tx *types.Tx) bool {
	// SimpleBlockFactory has no tx valid check.
	return true
}

// IsBlockValid checks the consensus level validity of a block.
func (s *SimpleBlockFactory) IsBlockValid(*types.Block, *types.Block) error {
	// SimpleBlockFactory has no block valid check.
	return nil
}

// QuitChan returns the channel from which consensus-related goroutines check
// when shutdown is initiated.
func (s *SimpleBlockFactory) QuitChan() chan interface{} {
	return s.quit
}

// UpdateStatus has nothging to do.
func (s *SimpleBlockFactory) UpdateStatus(block *types.Block) {
}

// BlockFactory returns s itself.
func (s *SimpleBlockFactory) BlockFactory() consensus.BlockFactory {
	return s
}

// NeedReorganization has nothing to do.
func (s *SimpleBlockFactory) NeedReorganization(rootNo, bestNo types.BlockNo) bool {
	return true
}

// Start run a simple block factory service.
func (s *SimpleBlockFactory) Start() {
	defer logger.Info().Msg("shutdown initiated. stop the service")

	for {
		select {
		case e := <-s.jobQueue:
			if prevBlock, ok := e.(*types.Block); ok {
				block, _, err := chain.GenerateBlock(s, prevBlock, s.txOp, time.Now().UnixNano())
				if err == chain.ErrQuit {
					return
				} else if err != nil {
					logger.Info().Err(err).Msg("failed to produce block")
					continue
				}
				logger.Info().Uint64("no", block.GetHeader().GetBlockNo()).Str("hash", block.ID()).
					Err(err).Msg("block produced")

				chain.ConnectBlock(s, block, nil)
			}
		case <-s.quit:
			return
		}
	}
}

// JobQueue returns the queue for block production triggering.
func (s *SimpleBlockFactory) JobQueue() chan<- interface{} {
	return s.jobQueue
}
