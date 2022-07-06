file(REMOVE_RECURSE
  "libuniformGenerator.a"
  "libuniformGenerator.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/uniformGenerator.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
