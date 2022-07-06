file(REMOVE_RECURSE
  "libgenerator.a"
  "libgenerator.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/generator.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
