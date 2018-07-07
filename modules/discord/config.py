def can_build(env, platform):
    return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
    env.Append(CPPPATH = '#modules/discord/boost/')
    env.Append(LIBPATH = '#modules/discord/boost/libs/lib64-msvc-14.1/')
    env.Append(LINKFLAGS = 'libboost_date_time-vc141-mt-s-x64-1_67.lib')
