// Code generated by MockGen. DO NOT EDIT.
// Source: github.com/aergoio/aergo/p2p/p2pcommon (interfaces: NTContainer,NetworkTransport)

// Package mock_p2pcommon is a generated GoMock package.
package p2pmock

import (
	context "context"
	p2pcommon "github.com/aergoio/aergo/p2p/p2pcommon"
	types "github.com/aergoio/aergo/types"
	gomock "github.com/golang/mock/gomock"
	connmgr "github.com/libp2p/go-libp2p-core/connmgr"
	network "github.com/libp2p/go-libp2p-core/network"
	peer "github.com/libp2p/go-libp2p-core/peer"
	peerstore "github.com/libp2p/go-libp2p-core/peerstore"
	protocol "github.com/libp2p/go-libp2p-core/protocol"
	go_multiaddr "github.com/multiformats/go-multiaddr"
	reflect "reflect"
	time "time"
)

// MockNTContainer is a mock of NTContainer interface
type MockNTContainer struct {
	ctrl     *gomock.Controller
	recorder *MockNTContainerMockRecorder
}

// MockNTContainerMockRecorder is the mock recorder for MockNTContainer
type MockNTContainerMockRecorder struct {
	mock *MockNTContainer
}

// NewMockNTContainer creates a new mock instance
func NewMockNTContainer(ctrl *gomock.Controller) *MockNTContainer {
	mock := &MockNTContainer{ctrl: ctrl}
	mock.recorder = &MockNTContainerMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockNTContainer) EXPECT() *MockNTContainerMockRecorder {
	return m.recorder
}

// ChainID mocks base method
func (m *MockNTContainer) ChainID() *types.ChainID {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "ChainID")
	ret0, _ := ret[0].(*types.ChainID)
	return ret0
}

// ChainID indicates an expected call of ChainID
func (mr *MockNTContainerMockRecorder) ChainID() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "ChainID", reflect.TypeOf((*MockNTContainer)(nil).ChainID))
}

// GetNetworkTransport mocks base method
func (m *MockNTContainer) GetNetworkTransport() p2pcommon.NetworkTransport {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "GetNetworkTransport")
	ret0, _ := ret[0].(p2pcommon.NetworkTransport)
	return ret0
}

// GetNetworkTransport indicates an expected call of GetNetworkTransport
func (mr *MockNTContainerMockRecorder) GetNetworkTransport() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetNetworkTransport", reflect.TypeOf((*MockNTContainer)(nil).GetNetworkTransport))
}

// SelfMeta mocks base method
func (m *MockNTContainer) SelfMeta() p2pcommon.PeerMeta {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "SelfMeta")
	ret0, _ := ret[0].(p2pcommon.PeerMeta)
	return ret0
}

// SelfMeta indicates an expected call of SelfMeta
func (mr *MockNTContainerMockRecorder) SelfMeta() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "SelfMeta", reflect.TypeOf((*MockNTContainer)(nil).SelfMeta))
}

// MockNetworkTransport is a mock of NetworkTransport interface
type MockNetworkTransport struct {
	ctrl     *gomock.Controller
	recorder *MockNetworkTransportMockRecorder
}

// MockNetworkTransportMockRecorder is the mock recorder for MockNetworkTransport
type MockNetworkTransportMockRecorder struct {
	mock *MockNetworkTransport
}

// NewMockNetworkTransport creates a new mock instance
func NewMockNetworkTransport(ctrl *gomock.Controller) *MockNetworkTransport {
	mock := &MockNetworkTransport{ctrl: ctrl}
	mock.recorder = &MockNetworkTransportMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockNetworkTransport) EXPECT() *MockNetworkTransportMockRecorder {
	return m.recorder
}

// AddStreamHandler mocks base method
func (m *MockNetworkTransport) AddStreamHandler(arg0 protocol.ID, arg1 network.StreamHandler) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "AddStreamHandler", arg0, arg1)
}

// AddStreamHandler indicates an expected call of AddStreamHandler
func (mr *MockNetworkTransportMockRecorder) AddStreamHandler(arg0, arg1 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "AddStreamHandler", reflect.TypeOf((*MockNetworkTransport)(nil).AddStreamHandler), arg0, arg1)
}

// Addrs mocks base method
func (m *MockNetworkTransport) Addrs() []go_multiaddr.Multiaddr {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Addrs")
	ret0, _ := ret[0].([]go_multiaddr.Multiaddr)
	return ret0
}

// Addrs indicates an expected call of Addrs
func (mr *MockNetworkTransportMockRecorder) Addrs() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Addrs", reflect.TypeOf((*MockNetworkTransport)(nil).Addrs))
}

// Close mocks base method
func (m *MockNetworkTransport) Close() error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Close")
	ret0, _ := ret[0].(error)
	return ret0
}

// Close indicates an expected call of Close
func (mr *MockNetworkTransportMockRecorder) Close() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Close", reflect.TypeOf((*MockNetworkTransport)(nil).Close))
}

// ClosePeerConnection mocks base method
func (m *MockNetworkTransport) ClosePeerConnection(arg0 peer.ID) bool {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "ClosePeerConnection", arg0)
	ret0, _ := ret[0].(bool)
	return ret0
}

// ClosePeerConnection indicates an expected call of ClosePeerConnection
func (mr *MockNetworkTransportMockRecorder) ClosePeerConnection(arg0 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "ClosePeerConnection", reflect.TypeOf((*MockNetworkTransport)(nil).ClosePeerConnection), arg0)
}

// ConnManager mocks base method
func (m *MockNetworkTransport) ConnManager() connmgr.ConnManager {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "ConnManager")
	ret0, _ := ret[0].(connmgr.ConnManager)
	return ret0
}

// ConnManager indicates an expected call of ConnManager
func (mr *MockNetworkTransportMockRecorder) ConnManager() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "ConnManager", reflect.TypeOf((*MockNetworkTransport)(nil).ConnManager))
}

// Connect mocks base method
func (m *MockNetworkTransport) Connect(arg0 context.Context, arg1 peer.AddrInfo) error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Connect", arg0, arg1)
	ret0, _ := ret[0].(error)
	return ret0
}

// Connect indicates an expected call of Connect
func (mr *MockNetworkTransportMockRecorder) Connect(arg0, arg1 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Connect", reflect.TypeOf((*MockNetworkTransport)(nil).Connect), arg0, arg1)
}

// FindPeer mocks base method
func (m *MockNetworkTransport) FindPeer(arg0 peer.ID) bool {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "FindPeer", arg0)
	ret0, _ := ret[0].(bool)
	return ret0
}

// FindPeer indicates an expected call of FindPeer
func (mr *MockNetworkTransportMockRecorder) FindPeer(arg0 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "FindPeer", reflect.TypeOf((*MockNetworkTransport)(nil).FindPeer), arg0)
}

// GetAddressesOfPeer mocks base method
func (m *MockNetworkTransport) GetAddressesOfPeer(arg0 peer.ID) []string {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "GetAddressesOfPeer", arg0)
	ret0, _ := ret[0].([]string)
	return ret0
}

// GetAddressesOfPeer indicates an expected call of GetAddressesOfPeer
func (mr *MockNetworkTransportMockRecorder) GetAddressesOfPeer(arg0 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetAddressesOfPeer", reflect.TypeOf((*MockNetworkTransport)(nil).GetAddressesOfPeer), arg0)
}

// GetOrCreateStream mocks base method
func (m *MockNetworkTransport) GetOrCreateStream(arg0 p2pcommon.PeerMeta, arg1 ...protocol.ID) (network.Stream, error) {
	m.ctrl.T.Helper()
	varargs := []interface{}{arg0}
	for _, a := range arg1 {
		varargs = append(varargs, a)
	}
	ret := m.ctrl.Call(m, "GetOrCreateStream", varargs...)
	ret0, _ := ret[0].(network.Stream)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// GetOrCreateStream indicates an expected call of GetOrCreateStream
func (mr *MockNetworkTransportMockRecorder) GetOrCreateStream(arg0 interface{}, arg1 ...interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	varargs := append([]interface{}{arg0}, arg1...)
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetOrCreateStream", reflect.TypeOf((*MockNetworkTransport)(nil).GetOrCreateStream), varargs...)
}

// GetOrCreateStreamWithTTL mocks base method
func (m *MockNetworkTransport) GetOrCreateStreamWithTTL(arg0 p2pcommon.PeerMeta, arg1 time.Duration, arg2 ...protocol.ID) (network.Stream, error) {
	m.ctrl.T.Helper()
	varargs := []interface{}{arg0, arg1}
	for _, a := range arg2 {
		varargs = append(varargs, a)
	}
	ret := m.ctrl.Call(m, "GetOrCreateStreamWithTTL", varargs...)
	ret0, _ := ret[0].(network.Stream)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// GetOrCreateStreamWithTTL indicates an expected call of GetOrCreateStreamWithTTL
func (mr *MockNetworkTransportMockRecorder) GetOrCreateStreamWithTTL(arg0, arg1 interface{}, arg2 ...interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	varargs := append([]interface{}{arg0, arg1}, arg2...)
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetOrCreateStreamWithTTL", reflect.TypeOf((*MockNetworkTransport)(nil).GetOrCreateStreamWithTTL), varargs...)
}

// ID mocks base method
func (m *MockNetworkTransport) ID() peer.ID {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "ID")
	ret0, _ := ret[0].(peer.ID)
	return ret0
}

// ID indicates an expected call of ID
func (mr *MockNetworkTransportMockRecorder) ID() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "ID", reflect.TypeOf((*MockNetworkTransport)(nil).ID))
}

// Mux mocks base method
func (m *MockNetworkTransport) Mux() protocol.Switch {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Mux")
	ret0, _ := ret[0].(protocol.Switch)
	return ret0
}

// Mux indicates an expected call of Mux
func (mr *MockNetworkTransportMockRecorder) Mux() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Mux", reflect.TypeOf((*MockNetworkTransport)(nil).Mux))
}

// Network mocks base method
func (m *MockNetworkTransport) Network() network.Network {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Network")
	ret0, _ := ret[0].(network.Network)
	return ret0
}

// Network indicates an expected call of Network
func (mr *MockNetworkTransportMockRecorder) Network() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Network", reflect.TypeOf((*MockNetworkTransport)(nil).Network))
}

// NewStream mocks base method
func (m *MockNetworkTransport) NewStream(arg0 context.Context, arg1 peer.ID, arg2 ...protocol.ID) (network.Stream, error) {
	m.ctrl.T.Helper()
	varargs := []interface{}{arg0, arg1}
	for _, a := range arg2 {
		varargs = append(varargs, a)
	}
	ret := m.ctrl.Call(m, "NewStream", varargs...)
	ret0, _ := ret[0].(network.Stream)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// NewStream indicates an expected call of NewStream
func (mr *MockNetworkTransportMockRecorder) NewStream(arg0, arg1 interface{}, arg2 ...interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	varargs := append([]interface{}{arg0, arg1}, arg2...)
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "NewStream", reflect.TypeOf((*MockNetworkTransport)(nil).NewStream), varargs...)
}

// Peerstore mocks base method
func (m *MockNetworkTransport) Peerstore() peerstore.Peerstore {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Peerstore")
	ret0, _ := ret[0].(peerstore.Peerstore)
	return ret0
}

// Peerstore indicates an expected call of Peerstore
func (mr *MockNetworkTransportMockRecorder) Peerstore() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Peerstore", reflect.TypeOf((*MockNetworkTransport)(nil).Peerstore))
}

// RemoveStreamHandler mocks base method
func (m *MockNetworkTransport) RemoveStreamHandler(arg0 protocol.ID) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "RemoveStreamHandler", arg0)
}

// RemoveStreamHandler indicates an expected call of RemoveStreamHandler
func (mr *MockNetworkTransportMockRecorder) RemoveStreamHandler(arg0 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "RemoveStreamHandler", reflect.TypeOf((*MockNetworkTransport)(nil).RemoveStreamHandler), arg0)
}

// SelfMeta mocks base method
func (m *MockNetworkTransport) SelfMeta() p2pcommon.PeerMeta {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "SelfMeta")
	ret0, _ := ret[0].(p2pcommon.PeerMeta)
	return ret0
}

// SelfMeta indicates an expected call of SelfMeta
func (mr *MockNetworkTransportMockRecorder) SelfMeta() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "SelfMeta", reflect.TypeOf((*MockNetworkTransport)(nil).SelfMeta))
}

// SetStreamHandler mocks base method
func (m *MockNetworkTransport) SetStreamHandler(arg0 protocol.ID, arg1 network.StreamHandler) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "SetStreamHandler", arg0, arg1)
}

// SetStreamHandler indicates an expected call of SetStreamHandler
func (mr *MockNetworkTransportMockRecorder) SetStreamHandler(arg0, arg1 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "SetStreamHandler", reflect.TypeOf((*MockNetworkTransport)(nil).SetStreamHandler), arg0, arg1)
}

// SetStreamHandlerMatch mocks base method
func (m *MockNetworkTransport) SetStreamHandlerMatch(arg0 protocol.ID, arg1 func(string) bool, arg2 network.StreamHandler) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "SetStreamHandlerMatch", arg0, arg1, arg2)
}

// SetStreamHandlerMatch indicates an expected call of SetStreamHandlerMatch
func (mr *MockNetworkTransportMockRecorder) SetStreamHandlerMatch(arg0, arg1, arg2 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "SetStreamHandlerMatch", reflect.TypeOf((*MockNetworkTransport)(nil).SetStreamHandlerMatch), arg0, arg1, arg2)
}

// Start mocks base method
func (m *MockNetworkTransport) Start() error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Start")
	ret0, _ := ret[0].(error)
	return ret0
}

// Start indicates an expected call of Start
func (mr *MockNetworkTransportMockRecorder) Start() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Start", reflect.TypeOf((*MockNetworkTransport)(nil).Start))
}

// Stop mocks base method
func (m *MockNetworkTransport) Stop() error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Stop")
	ret0, _ := ret[0].(error)
	return ret0
}

// Stop indicates an expected call of Stop
func (mr *MockNetworkTransportMockRecorder) Stop() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Stop", reflect.TypeOf((*MockNetworkTransport)(nil).Stop))
}
