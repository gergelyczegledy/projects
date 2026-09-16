package hu.bme.aut.android.macroapp.data.datasource

import hu.bme.aut.android.macroapp.domain.data.Macro

interface IMacroRepository {
    suspend fun getAllMacros() : List<Macro>

    suspend fun deleteMacro(macro: Macro)

    suspend fun upsertMacro(macro: Macro)
}