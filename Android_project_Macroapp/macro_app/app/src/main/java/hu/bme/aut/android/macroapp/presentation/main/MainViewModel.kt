package hu.bme.aut.android.macroapp.presentation.main

import android.content.Context
import android.content.Intent
import android.provider.Settings
import android.widget.Toast
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import dagger.hilt.android.lifecycle.HiltViewModel
import hu.bme.aut.android.macroapp.data.datasource.IMacroRepository
import hu.bme.aut.android.macroapp.domain.data.Macro
import hu.bme.aut.android.macroapp.domain.service.MacroAccessibilityService
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class MainViewModel @Inject constructor(
    val macroRepository: IMacroRepository
) : ViewModel() {
    private val _macros = MutableStateFlow<List<Macro>>(emptyList())
    val macros: StateFlow<List<Macro>> = _macros

    init {
        viewModelScope.launch {
            _macros.value = macroRepository.getAllMacros()
        }
    }

    private companion object {
        var NextID = 1
    }

    private fun getNextID(): Int {
        var newNextId = NextID
        if (_macros.value.isNotEmpty()) {
            newNextId = _macros.value.maxOf { macro -> macro.id } + 1
        }
        NextID = newNextId + 1
        return newNextId
    }

    fun updateMacro(id: Int, newMacro: Macro){
        _macros.value = _macros.value.map { if (it.id == id) newMacro else it }
        viewModelScope.launch {
            macroRepository.upsertMacro(newMacro)
        }
    }

    fun addMacro(macro: Macro){
        var newNextId = getNextID()
        val newMacro = macro.copy(id = newNextId)
        _macros.value = _macros.value + newMacro

        viewModelScope.launch {
            macroRepository.upsertMacro(newMacro)
        }
    }

    fun deleteMacro(id: Int){
        val macroToDelete = _macros.value.find { it.id == id }
        _macros.value = _macros.value.filter { it.id != id }
        if (macroToDelete != null) {
            viewModelScope.launch {
                macroRepository.deleteMacro(macroToDelete)
            }
        }
    }

    fun runMacro(macro: Macro, context: Context){
        val service = MacroAccessibilityService.instance
        if (service != null) {
            val homeIntent = Intent(Intent.ACTION_MAIN).apply {
                addCategory(Intent.CATEGORY_HOME)
                flags = Intent.FLAG_ACTIVITY_NEW_TASK
            }
            context.startActivity(homeIntent)
            service.playMacro(macro)
        } else {
            Toast.makeText(context, "Enable background macro service!", Toast.LENGTH_LONG).show()
            context.startActivity(Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS).apply {
                addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
            })
        }
    }
}