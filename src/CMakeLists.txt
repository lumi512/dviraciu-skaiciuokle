add_executable(dvpusk
	main.cpp
	menu.cpp
	menu_base.cpp
	printutils.cpp
	order.cpp
	order_price.cpp
	order_stringify.cpp
)

add_custom_target(copy_file ALL)
add_custom_command(
	TARGET copy_file POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/uzsakymas.txt ${CMAKE_CURRENT_BINARY_DIR}
)