package hu.bme.aut.android.macroapp.data.entity

import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey
import hu.bme.aut.android.macroapp.domain.data.Direction

@Entity(
    tableName = "actions",
    foreignKeys = [ForeignKey(
        entity = MacroEntity::class,
        parentColumns = ["databaseMacroId"],
        childColumns = ["ownersDatabaseMacroId"],
        onDelete = ForeignKey.CASCADE
    )]
)
data class ActionEntity(
    @PrimaryKey(autoGenerate = true)
    val databaseActionId: Int = 0,
    val ownersDatabaseMacroId: Int,
    val actionType: String,

    val direction: Direction? = null,

    val clickX: Int? = null,
    val clickY: Int? = null,

    val waitMilliseconds: Int? = null,

    val originalActionId: Int
    // val orderIndex: Int
)