package hu.bme.aut.android.macroapp.data.datasource

import hu.bme.aut.android.macroapp.domain.data.Action
import hu.bme.aut.android.macroapp.domain.data.Direction
import hu.bme.aut.android.macroapp.domain.data.Macro

class MacroRepositoryDummyImpl : IMacroRepository {
    override suspend fun getAllMacros(): List<Macro> {
        val newList: MutableList<Macro> = mutableListOf()
        var newMacro = Macro(
            id = 1,
            name = "Macro #${1}",
            actions = listOf(
                Action.Swipe(Direction.UP, 1),
                Action.Click(x = 100, y = 200, id = 2),
                Action.Wait(milliseconds = 1000, id = 3)
            )
        )
        newList.add(newMacro)
        newMacro = Macro(
            id = 2,
            name = "Macro #${2}",
            actions = listOf(
                Action.Swipe(Direction.DOWN, 1),
                Action.Click(x = 1, y = 1, id = 1),
                Action.Wait(milliseconds = 1, id = 1)
            )
        )
        newList.add(newMacro)
        newMacro = Macro(
            id = 3,
            name = "Macro #${3}",
            actions = listOf(
                Action.Swipe(Direction.RIGHT, 1),
                Action.Click(x = 1000, y = 1250, id = 2),
                Action.Wait(milliseconds = 10005, id = 3)
            )
        )
        newList.add(newMacro)
        return newList
    }
    override suspend fun deleteMacro(macro: Macro) {

    }
    override suspend fun upsertMacro(macro: Macro) {

    }
}