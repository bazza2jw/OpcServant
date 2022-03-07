# Node Flow definitons
add_definitions(-DUSE_WT)
set(NODEFLOWLIBS  -lNodeFlow)
link_directories(/usr/local/NodeFlow/lib)
include_directories(/usr/local/NodeFlow/include)
