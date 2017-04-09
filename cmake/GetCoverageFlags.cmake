if(__get_coverage_flags)
  return()
endif()
set(__get_coverage_flags)

include(AddFlagIfSupported)

function(get_coverage_flags OUTPUT)
  add_flag_if_supported("-g"              FLAGS)
  add_flag_if_supported("-O0"             FLAGS)
  add_flag_if_supported("-fprofile-arcs"  FLAGS)
  add_flag_if_supported("-ftest-coverage" FLAGS)

  set(${OUTPUT} ${FLAGS} PARENT_SCOPE)
endfunction()
