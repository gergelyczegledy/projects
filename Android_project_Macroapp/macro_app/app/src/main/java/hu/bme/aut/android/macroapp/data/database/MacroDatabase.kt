package hu.bme.aut.android.macroapp.data.database

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import hu.bme.aut.android.macroapp.data.dao.MacroDao
import hu.bme.aut.android.macroapp.data.entity.ActionEntity
import hu.bme.aut.android.macroapp.data.entity.MacroEntity

@Database(
    entities = [
        MacroEntity::class,
        ActionEntity::class
    ],
    version = 1,
    exportSchema = false
)
abstract class MacroDatabase : RoomDatabase() {
    abstract fun MacroDao(): MacroDao

    companion object {
        @Volatile
        private var INSTANCE: MacroDatabase? = null

        fun getDatabase(context: Context): MacroDatabase {
            return INSTANCE ?: synchronized(this) {
                val instance = Room.databaseBuilder(
                    context.applicationContext,
                    MacroDatabase::class.java,
                    "macro_database"
                ).build()
                INSTANCE = instance
                instance
            }
        }
    }
}