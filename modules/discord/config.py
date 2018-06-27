def can_build(env, platform):
    return True

def configure(env):
    env.Append(CPPPATH="#modules/discord/sdk/win64/include/")

    # Only works for windows on the VS prompt, but it's all i'm testing atm
    env.Append(LINKFLAGS=["discord-rpc.lib"])
    env.Append(LIBPATH="#modules/discord/sdk/win64/lib")
    
    
