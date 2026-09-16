package hu.bme.aut.android.macroapp.domain.data

sealed class Action(val id: Int) {
    class Swipe(val direction: Direction, id: Int) : Action(id = id)
    class Click(val x: Int, val y: Int, id: Int) : Action(id = id)
    class Wait(val milliseconds: Int, id: Int) : Action(id = id)
}

enum class Direction {
    UP, DOWN, LEFT, RIGHT
}