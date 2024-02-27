# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Automagically manage the resources
set(CMAKE_AUTORCC ON)

add_subdirectory(apps)
add_subdirectory(libs)
