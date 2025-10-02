project(RoboForge)

add_executable(simulator
        simulator/_time.cpp
        simulator/funcs.cpp
        simulator/hserial.cpp
        simulator/simulator.cpp
        simulator/engine.cpp
        simulator/sock.cpp
        simulator/components/pin.cpp
        simulator/components/component.cpp
        simulator/components/ard.cpp
        simulator/components/wire.cpp
        simulator/components/tempsens.cpp
        simulator/components/led.cpp
        simulator/components/irsensor.cpp
        simulator/components/breadboard.cpp
)