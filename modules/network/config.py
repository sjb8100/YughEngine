def can_build(env, platform):
    True

def configure(env):

    ### discord includes
    discordsdk = "#modules/network/discord/sdk/"
    env.Append(CPPPATH = discordsdk + "win64/include/")
    

    if env["platform"] == "windows":
        if env["bits"]=="32":
            env.Append(LIBPATH = discordsdk + "win32/lib")
        else: # 64 bit
            env.Append(LIBPATH = discordsdk + "win64/lib")

        if env["CC"] == "cl": # Visual Studio builder support
            env.Append(LINKFLAGS="discord-rpc.lib") 
        else: # gcc support
            env.Append(LIBS="discord-rpc")

    elif env["platform"] == "osx":
        env.Append(LIBS="libdiscord-rpc")
        env.Append(LIBPATH = discordsdk + "osx/lib")

    ### Steam includes
    steamsdk = "#modules/network/steam/sdk/redistributable_bin/"
    env.Append(CPPPATH = "#modules/godotsteam/sdk/public/")

    # Linux handling
    if env["platform"] == "x11":
        env.Append(LIBS = ["steam_api"])
    #env.Append(RPATH=["."])
        if env["bits"]=="32":
            env.Append(RPATH = env.Literal('\\$$ORIGIN/linux32'))
            env.Append(LIBPATH = steamsdk + "linux32")
        else: # 64 bit
            env.Append(RPATH=env.Literal('\\$$ORIGIN/linux64'))
            env.Append(LIBPATH = steamsdk + "linux64")
    elif env["platform"] == "windows": # mostly VisualStudio
        if env["CC"] == "cl":
            if env["bits"] == "32":
                env.Append(LINKFLAGS = "steam_api.lib")
                env.Append(LIBPATH = steamsdk)
            else: # 64 bit
                env.Append(LINKFLAGS = "steam_api64.lib")
                env.Append(LIBPATH = steamsdk + "win64")
        # mostly "gcc"
        else:
            if env["bits"]=="32":
                env.Append(LIBS = "steam_api")
                env.Append(LIBPATH = steamsdk)
            else: # 64 bit
                env.Append(LIBS = "steam_api64")
                env.Append(LIBPATH = steamsdk + "win64")
    elif env["platform"] == "osx":
        env.Append(LIBS = "steam_api")
        env.Append(LIBPATH = steamsdk + "osx32")
