package hu.bme.aut.android.macroapp.data.entity

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(
    tableName = "macros"
)
data class MacroEntity(
    @PrimaryKey(autoGenerate = true)
    val databaseMacroId: Int = 0,
    val id: Int,
    val name: String
)