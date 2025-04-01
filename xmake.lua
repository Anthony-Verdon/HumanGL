set_languages("cxx17")

set_toolchains("clang")
set_warnings("allextra", "error")
set_optimize("fastest")
set_symbols("debug")

add_requires("imgui", {configs = {glfw = true, opengl3 = true}})

includes("submodules/Engine")

target("HumanGL")
    set_targetdir("./")
    set_kind("binary")
    add_files("srcs/**.cpp")
    add_includedirs("srcs")
    add_packages("imgui")
    add_deps("Engine")