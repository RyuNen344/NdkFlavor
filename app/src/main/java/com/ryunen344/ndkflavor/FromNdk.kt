package com.ryunen344.ndkflavor

class FromNdk {

    external fun getString(): String

    external fun getSigFromNdk(buildVersion: Int): Int

    companion object {
        init {
            System.loadLibrary("hello-lib")
        }
    }
}
