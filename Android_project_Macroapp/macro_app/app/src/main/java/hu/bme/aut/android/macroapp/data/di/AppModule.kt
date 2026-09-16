package hu.bme.aut.android.macroapp.data.di

import android.content.Context
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import hu.bme.aut.android.macroapp.data.dao.MacroDao
import hu.bme.aut.android.macroapp.data.database.MacroDatabase
import hu.bme.aut.android.macroapp.data.datasource.IMacroRepository
import hu.bme.aut.android.macroapp.data.datasource.MacroRepositoryDummyImpl
import hu.bme.aut.android.macroapp.data.datasource.MacroRepositoryRoomImpl
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object AppModule {
    @Provides
    @Singleton
    fun provideDatabase(@ApplicationContext appContext: Context): MacroDatabase {
        return MacroDatabase.getDatabase(appContext)
    }
    @Provides
    @Singleton
    fun provideTopScoreDao(database: MacroDatabase): MacroDao {
        return database.MacroDao()
    }
    @Provides
    @Singleton
    fun provideMacroRepository(macroDao: MacroDao): IMacroRepository {
        return MacroRepositoryRoomImpl(macroDao)
    }
}