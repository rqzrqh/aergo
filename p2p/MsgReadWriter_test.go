package p2p

import "github.com/stretchr/testify/mock"

// MockMsgReadWriter is an autogenerated mock type for the MsgReadWriter type
type MockMsgReadWriter struct {
	mock.Mock
}

// ReadMsg provides a mock function with given fields:
func (_m *MockMsgReadWriter) ReadMsg() (Message, error) {
	ret := _m.Called()

	var r0 Message
	if rf, ok := ret.Get(0).(func() Message); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(Message)
		}
	}

	var r1 error
	if rf, ok := ret.Get(1).(func() error); ok {
		r1 = rf()
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// WriteMsg provides a mock function with given fields: header
func (_m *MockMsgReadWriter) WriteMsg(header Message) error {
	ret := _m.Called(header)

	var r0 error
	if rf, ok := ret.Get(0).(func(Message) error); ok {
		r0 = rf(header)
	} else {
		r0 = ret.Error(0)
	}

	return r0
}