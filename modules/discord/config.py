def can_build(env, platform):
    return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
    env.Append(CPPPATH = '#modules/discord/howard/include')
