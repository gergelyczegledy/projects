package hu.bme.aut.android.macroapp.domain.service

import android.accessibilityservice.AccessibilityService
import android.view.accessibility.AccessibilityEvent
import android.accessibilityservice.GestureDescription
import android.content.Intent
import android.graphics.Path
import android.os.Handler
import android.os.Looper
import android.util.Log
import hu.bme.aut.android.macroapp.domain.data.Macro
import hu.bme.aut.android.macroapp.domain.data.Action
import hu.bme.aut.android.macroapp.domain.data.Direction
import java.util.concurrent.atomic.AtomicBoolean

class MacroAccessibilityService : AccessibilityService() {
    companion object {
        var instance: MacroAccessibilityService? = null
    }

    override fun onServiceConnected() {
        instance = this
    }

    override fun onAccessibilityEvent(event: AccessibilityEvent?) {

    }

    override fun onInterrupt() {}

    fun playMacro(macro: Macro) {
        Handler(Looper.getMainLooper()).postDelayed({
            Thread {
                Log.d("MacroService", "Starting macro playback on new thread.")
                macro.actions.forEachIndexed { index, action ->
                    Log.d("MacroService", "Executing action #${index + 1}: $action")
                    when (action) {
                        is Action.Click -> {
                            performClick(action.x, action.y)
                            Thread.sleep(300)
                        }
                        is Action.Swipe -> {
                            Log.d("MacroService", "Attempting Swipe ${action.direction}")
                            performSwipe(action.direction)
                            Thread.sleep(400)
                        }
                        is Action.Wait -> {
                            Thread.sleep(action.milliseconds.toLong())
                        }
                    }
                }
            }.start()
        }, 1000)
    }

    private fun performClick(x: Int, y: Int) {
        val path = Path().apply { moveTo(x.toFloat(), y.toFloat()) }
        val gesture = GestureDescription.Builder()
            .addStroke(GestureDescription.StrokeDescription(path, 0, 100))
            .build()
        dispatchGesture(gesture, null, null)
    }

    private fun performSwipe(direction: Direction) {
        val displayMetrics = resources.displayMetrics
        val width = displayMetrics.widthPixels
        val height = displayMetrics.heightPixels

        val (x1, y1, x2, y2) = when (direction) {
            Direction.UP -> listOf(width / 2, height * 3 / 4, width / 2, height / 4)
            Direction.DOWN -> listOf(width / 2, height / 4, width / 2, height * 3 / 4)
            Direction.LEFT -> listOf(width * 3 / 4, height / 2, width / 4, height / 2)
            Direction.RIGHT -> listOf(width / 4, height / 2, width * 3 / 4, height / 2)
        }

        val path = Path().apply {
            moveTo(x1.toFloat(), y1.toFloat())
            lineTo(x2.toFloat(), y2.toFloat())
        }

        val gesture = GestureDescription.Builder()
            .addStroke(GestureDescription.StrokeDescription(path, 0, 250))
            .build()
        dispatchGesture(gesture, object : GestureResultCallback() {
            override fun onCompleted(gestureDescription: GestureDescription?) {
                super.onCompleted(gestureDescription)
                Log.d("MacroService", "Gesture completed")
            }
            override fun onCancelled(gestureDescription: GestureDescription?) {
                super.onCancelled(gestureDescription)
                Log.d("MacroService", "Gesture cancelled")
            }
        }, null)
    }

    private fun returnToApp() {
        val launchIntent = packageManager.getLaunchIntentForPackage("hu.bme.aut.android.macroapp")
        launchIntent?.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
        startActivity(launchIntent)
    }
}