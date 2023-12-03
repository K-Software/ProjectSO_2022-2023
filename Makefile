# ---------------------------------------------------------------------------- #
# ecu.out
# ---------------------------------------------------------------------------- #
ecu.out: src/ecu.c socket_utils.o log.o string_utils.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# front_windshield_camera.out
# ---------------------------------------------------------------------------- #
front_windshield_camera.out: src/front_windshield_camera.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# steer_by_wire.out
# ---------------------------------------------------------------------------- #
steer_by_wire.out: src/steer_by_wire.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# throttle_control
# ---------------------------------------------------------------------------- #
throttle_control.out: src/throttle_control.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# brake_by_wire
# ---------------------------------------------------------------------------- #
brake_by_wire.out: src/brake_by_wire.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# park_assist
# ---------------------------------------------------------------------------- #
park_assist.out: src/park_assist.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# hmi_output
# ---------------------------------------------------------------------------- #
hmi_output.out: src/hmi_output.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

# ---------------------------------------------------------------------------- #
# hmi_input
# ---------------------------------------------------------------------------- #
hmi_input.out: src/hmi_input.c socket_utils.o string_utils.o log.o
	cc $^ -o $@

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

all: ecu.out front_windshield_camera.out steer_by_wire.out throttle_control.out\
brake_by_wire.out park_assist.out hmi_output.out hmi_input.out

install:
	mkdir ./bin
	mkdir ./logs
	mkdir ./sockets
	mv *.out ./bin/
	rm *.o

uninstall:
	rm -rf ./bin
	rm -rf ./logs
	rm -rf ./sockets