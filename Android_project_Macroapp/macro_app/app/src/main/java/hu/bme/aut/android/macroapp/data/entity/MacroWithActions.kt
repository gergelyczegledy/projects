package hu.bme.aut.android.macroapp.data.entity

import androidx.room.Embedded
import androidx.room.Relation

data class MacroWithActions(
    @Embedded
    val macro: MacroEntity,

    @Relation(
        parentColumn = "databaseMacroId",
        entityColumn = "ownersDatabaseMacroId"
    )
    val actions: List<ActionEntity>
)