package com.ryunen344.ndkflavor

class FromNdk {

    external fun getString(): String

    companion object {
        init {
            System.loadLibrary("hello-lib")
        }
    }
}
