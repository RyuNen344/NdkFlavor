package com.ryunen344.ndkflavor

import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity(R.layout.activity_main) {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        findViewById<TextView>(R.id.view_text).apply {
            text = FromNdk().getString()
        }

        Log.e("TAG", "${FromNdk().getSigFromNdk(Build.VERSION.SDK_INT)}")
    }
}
