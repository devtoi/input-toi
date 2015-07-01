file(GLOB_RECURSE InputSources
	"Input.h"
	"Input.cpp"
	"TextInput.h"
	"TextInput.cpp"
	"KeyBindings.h"
	"KeyBindings.cpp"
	"KeyBindingCollection.h"
	"KeyBindingCollection.cpp"
	"Typedefs.h"
	"LogInput.h"
)

if(USE_CUSTOM_ALLOCATOR_HEADER_FOR_INPUT)
else(USE_CUSTOM_ALLOCATOR_HEADER_FOR_INPUT)
	list(APPEND InputSources "Alloc.h")
	add_definitions(-DINPUT_ALLOCATION_HEADER=${CUSTOM_ALLOCATOR_HEADER_FOR_INPUT})
endif(USE_CUSTOM_ALLOCATOR_HEADER_FOR_INPUT)

message(${CUSTOM_ALLOCATOR_HEADER_FOR_INPUT})
add_definitions(-DINPUT_DLL_EXPORT)
add_library(Input SHARED ${InputSources})
target_link_libraries(Input Utility ${SDL2Library} Memory)

install(
	TARGETS Input DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin/${OperatingSystemNameLowerCase}/${TargetArchitecture})
