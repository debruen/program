{
    "targets": [{
        "target_name": "ia",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'MACOSX_DEPLOYMENT_TARGET': '10.13',
                }
            }]
        ],
        "sources": [
            "cpp/main.cpp",
            "cpp/nodeclass.cpp",
            "cpp/wtc.cpp",
            "cpp/who.cpp",
            "cpp/ia.cpp",
            "cpp/image_filter.cpp",
            "cpp/image_synth.cpp",
            "cpp/image_out.cpp",
            "cpp/audio_filter.cpp",
            "cpp/audio_synth.cpp",
            "cpp/audio_out.cpp",
            "cpp/synthesis/functions.cpp",
            "cpp/synthesis/sine.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "/usr/",
            "/usr/local/lib",
            "/usr/local/include",
            "/usr/local/include/opencv4",
            "/usr/X11R6/include",
            "/usr/X11R6/lib",
            "/usr/local/opt/stk/lib"
        ],
        'libraries': [
            '-I/usr/X11R6/include',
            '-L/usr/X11R6/lib',
            '-lopencv_core',
            '-lopencv_imgproc',
            '-lopencv_highgui',
            '-lX11',
            '-lpng',
            '-ltiff',
            '-ljpeg',
            '-lstk',
            '-lfftw3'
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}
