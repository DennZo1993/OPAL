if(__add_flag_if_supported)
  return()
endif()
set(__add_flag_if_supported INCLUDED)

include(CheckCXXCompilerFlag)

function(add_flag_if_supported FLAG OUTPUT_PARAM)
  check_cxx_compiler_flag(${FLAG} COMPILER_SUPPORTS_FLAG)
  if(COMPILER_SUPPORTS_FLAG AND NOT MSVC)
    set(${OUTPUT_PARAM} ${${OUTPUT_PARAM}} ${FLAG} PARENT_SCOPE)
  else()
    message("-- C++ compiler doesn't support ${FLAG}")
  endif()
endfunction(add_flag_if_supported)
