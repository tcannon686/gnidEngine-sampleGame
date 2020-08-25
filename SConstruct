"""
\file SConstruct
\brief Builds the game and its dependencies
"""

# Set up the build environment.
env = Environment(
        CPPPATH = [
            '#/include',
            '#/deps',
            '#/deps/gnidEngine/include',
            '#/deps/gnidEngine/deps',
            '#/deps/gnidEngine/deps/glad/include'
        ],
        CPPFLAGS = '-g')

Export('env')

gnid = 'deps/gnidEngine/gnid.a'

game = env.SConscript(
        'src/SConscript',
        variant_dir = 'build/src',
        exports = 'gnid')

Return('game')

