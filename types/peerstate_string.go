// Code generated by "stringer -type=PeerState"; DO NOT EDIT.

package types

import "strconv"

func _() {
	// An "invalid array index" compiler error signifies that the constant values have changed.
	// Re-run the stringer command to generate them again.
	var x [1]struct{}
	_ = x[STARTING-0]
	_ = x[HANDSHAKING-1]
	_ = x[RUNNING-2]
	_ = x[STOPPING-3]
	_ = x[STOPPED-4]
	_ = x[DOWN-5]
}

const _PeerState_name = "STARTINGHANDSHAKINGRUNNINGSTOPPINGSTOPPEDDOWN"

var _PeerState_index = [...]uint8{0, 8, 19, 26, 34, 41, 45}

func (i PeerState) String() string {
	if i < 0 || i >= PeerState(len(_PeerState_index)-1) {
		return "PeerState(" + strconv.FormatInt(int64(i), 10) + ")"
	}
	return _PeerState_name[_PeerState_index[i]:_PeerState_index[i+1]]
}
