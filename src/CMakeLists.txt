target_sources(lake
    PRIVATE
    string_view.cpp
)

target_compile_options(lake PRIVATE -nostdinc++)
target_link_options(lake PRIVATE -nodefaultlibs)
