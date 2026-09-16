package hu.bme.aut.android.macroapp.domain.data

data class Macro(
    val id: Int = -1,
    val name: String = "MacroName",
    val actions: List<Action> = listOf<Action>(Action.Swipe(Direction.UP, -1))
)