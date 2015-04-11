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
	"Alloc.h"
	"LogInput.h"
)

add_definitions(-DINPUT_DLL_EXPORT)
add_library(Input SHARED ${InputSources})
target_link_libraries(Input Utility ${SDL2Library})

install(
	TARGETS Input DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin/${OperatingSystemNameLowerCase}/${TargetArchitecture})
