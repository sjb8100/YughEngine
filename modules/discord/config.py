def can_build(env, platform):
    return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
    env.Append(CPPPATH="#modules/discord/sdk/win64/include/")

    if env["platform"] == "windows":
        if env["bits"]=="32":
            env.Append(LIBPATH="#modules/discord/sdk/win32/lib")
        else: # 64 bit
            env.Append(LIBPATH="#modules/discord/sdk/win64/lib")

        if env["CC"] == "cl": # Visual Studio builder support
            env.Append(LINKFLAGS="discord-rpc.lib") 
        else: # gcc support
            env.Append(LIBS="discord-rpc")

    elif env["platform"] == "osx":
        env.Append(LIBS="libdiscord-rpc")
        env.Append(LIBPATH="#modules/discord/sdk/osx/lib")
