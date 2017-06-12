message("Qt-packeage should installed in /opt/Qt/current/")
set(Qt5_DIR "/opt/Qt/current/gcc_64/lib/cmake/")
find_package(Qt5 COMPONENTS Core Widgets Charts DataVisualization LinguistTools REQUIRED PATHS ${Qt5_DIR} NO_DEFAULT_PATH)
