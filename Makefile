# ---------------------------------------------------------------------------- #
# ecu.out
# ---------------------------------------------------------------------------- #
ecu.out: src/ecu.c socket_utils.o log.o string_utils.o
	cc $^ -o ./bin/$@

# ---------------------------------------------------------------------------- #
# front_windshield_camera.out
# ---------------------------------------------------------------------------- #
front_windshield_camera.out: src/front_windshield_camera.c socket_utils.o string_utils.o log.o
	cc $^ -o ./bin/$@

# ---------------------------------------------------------------------------- #
# steer_by_wire.out
# ---------------------------------------------------------------------------- #
steer_by_wire.out: src/steer_by_wire.c socket_utils.o string_utils.o log.o
	cc $^ -o ./bin/$@

# ---------------------------------------------------------------------------- #
# throttle_control
# ---------------------------------------------------------------------------- #
throttle_control.out: src/throttle_control.c socket_utils.o string_utils.o log.o
	cc $^ -o ./bin/$@

# ---------------------------------------------------------------------------- #
# socket_utils.o
# ---------------------------------------------------------------------------- #
socket_utils.o: src/socket_utils.c log.o
	cc -c $^

# ---------------------------------------------------------------------------- #
# string_utils
# ---------------------------------------------------------------------------- #
string_utils.o: src/string_utils.c
	cc -c $^

# ---------------------------------------------------------------------------- #
# log.o
# ---------------------------------------------------------------------------- #
log.o: src/log.c
	cc -c $^ 

all: ecu.out front_windshield_camera.out steer_by_wire.out throttle_control.out