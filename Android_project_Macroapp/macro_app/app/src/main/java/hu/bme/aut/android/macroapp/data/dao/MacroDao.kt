package hu.bme.aut.android.macroapp.data.dao

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import hu.bme.aut.android.macroapp.data.entity.ActionEntity
import hu.bme.aut.android.macroapp.data.entity.MacroEntity
import hu.bme.aut.android.macroapp.data.entity.MacroWithActions

import hu.bme.aut.android.macroapp.domain.data.Action
import hu.bme.aut.android.macroapp.domain.data.Macro

@Dao
abstract class MacroDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    abstract suspend fun insertMacroEntity(macro: MacroEntity): Long

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    abstract suspend fun insertActionEntities(actions: List<ActionEntity>)

    @Transaction
    open suspend fun upsertMacroWithActions(domainMacro: Macro) {
        val macroEntityToInsert = MacroEntity(
            id = domainMacro.id,
            name = domainMacro.name
        )

        var actualMacroId = getMacroWithActionsById(domainMacro.id)?.macro?.databaseMacroId
        if (actualMacroId != null){
            deleteActionsForMacro(actualMacroId)
        }
        if (actualMacroId == null){
            actualMacroId = insertMacroEntity(macroEntityToInsert).toInt()
        }
        if (domainMacro.actions.isNotEmpty()) {
            val actionEntities = domainMacro.actions.mapIndexed { index, domainAction ->
                ActionEntity(
                    ownersDatabaseMacroId = actualMacroId.toInt(),
                    actionType = when (domainAction) {
                        is Action.Swipe -> "SWIPE"
                        is Action.Click -> "CLICK"
                        is Action.Wait -> "WAIT"
                    },
                    direction = (domainAction as? Action.Swipe)?.direction,
                    clickX = (domainAction as? Action.Click)?.x,
                    clickY = (domainAction as? Action.Click)?.y,
                    waitMilliseconds = (domainAction as? Action.Wait)?.milliseconds,
                    originalActionId = domainAction.id
//                    orderInMacro = index
                )
            }
            insertActionEntities(actionEntities)
        }
    }

    // --- Query Operations ---

    @Transaction
    @Query("SELECT * FROM macros")
    abstract suspend fun getAllMacrosWithActions(): List<MacroWithActions>

    @Transaction
    @Query("SELECT * FROM macros WHERE id = :macroId")
    abstract suspend fun getMacroWithActionsById(macroId: Int): MacroWithActions?

    // --- Delete Operations ---

    @Query("DELETE FROM actions WHERE ownersDatabaseMacroId = :macroId")
    abstract suspend fun deleteActionsForMacro(macroId: Int)

    @Query("DELETE FROM macros WHERE id = :macroId")
    abstract suspend fun deleteMacroEntityById(macroId: Int)

    @Transaction
    open suspend fun deleteMacroAndActionsById(macroId: Int) {
        deleteMacroEntityById(macroId) // This should trigger CASCADE delete for actions
    }
}