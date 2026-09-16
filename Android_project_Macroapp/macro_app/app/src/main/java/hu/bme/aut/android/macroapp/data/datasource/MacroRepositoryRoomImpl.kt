package hu.bme.aut.android.macroapp.data.datasource

import hu.bme.aut.android.macroapp.data.dao.MacroDao
import hu.bme.aut.android.macroapp.domain.data.Action
import hu.bme.aut.android.macroapp.domain.data.Macro

class MacroRepositoryRoomImpl (
    private val macroDao: MacroDao
) : IMacroRepository {
    override suspend fun deleteMacro(macro: Macro) {
        macroDao.deleteMacroAndActionsById(macro.id)
    }

    override suspend fun upsertMacro(macro: Macro) {
        macroDao.upsertMacroWithActions(macro)
    }

    override suspend fun getAllMacros(): List<Macro> {
        val macrosWithActions = macroDao.getAllMacrosWithActions()
        var actions: List<Action> = emptyList()
        var macros: List<Macro> = emptyList()

        for (macroWithActions in macrosWithActions) {
            if (macroWithActions.actions.isNotEmpty()) {
                actions = macroWithActions.actions.map { actionEntity ->
                    when (actionEntity.actionType) {
                        "SWIPE" -> Action.Swipe(
                            direction = actionEntity.direction!!,
                            id = actionEntity.originalActionId
                        )
                        "CLICK" -> Action.Click(
                            x = actionEntity.clickX!!,
                            y = actionEntity.clickY!!,
                            id = actionEntity.originalActionId
                        )
                        "WAIT" -> Action.Wait(
                            milliseconds = actionEntity.waitMilliseconds!!,
                            id = actionEntity.originalActionId
                        )
                        else -> throw IllegalArgumentException("Unknown action type: ${actionEntity.actionType}")
                    }
                }
            }
            val macro = Macro(
                id = macroWithActions.macro.id,
                name = macroWithActions.macro.name,
                actions = actions
            )
            macros += macro
        }
        return macros
    }
}