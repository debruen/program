{
    "targets": [{
        "target_name": "ia",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                    '-std': 'c++11',
                    '-stdlib': 'libc++',
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'MACOSX_DEPLOYMENT_TARGET': '12.1'
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
            "/usr/include/lib",
            "/opt/homebrew/lib",
            "/opt/homebrew/include",
            "/opt/homebrew/include/opencv4"
        ],
        'libraries': [
            '-L/opt/homebrew/lib',
            '-lopencv_core',
            '-lopencv_imgproc',
            '-lopencv_highgui',
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
