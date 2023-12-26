target_sources(lake
    PRIVATE
    dummy.cpp
)

target_compile_options(lake PRIVATE -nostdinc++)
target_link_options(lake PRIVATE -nodefaultlibs)
