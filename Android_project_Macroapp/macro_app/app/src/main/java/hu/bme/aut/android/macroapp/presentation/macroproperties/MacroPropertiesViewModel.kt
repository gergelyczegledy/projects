package hu.bme.aut.android.macroapp.presentation.macroproperties

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import hu.bme.aut.android.macroapp.domain.data.Macro
import hu.bme.aut.android.macroapp.presentation.main.MainViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

class MacroPropertiesViewModel(
    private val mainViewModel: MainViewModel
) : ViewModel(){
    val _macro: MutableStateFlow<Macro> = MutableStateFlow(Macro())
    val macro: StateFlow<Macro> = _macro.asStateFlow()

    fun setMacro(newMacro: Macro?) {
        if (newMacro != null)
            _macro.value = newMacro
    }

    fun updateOrAddMacro() {
        if (_macro.value.id == -1)
            mainViewModel.addMacro(_macro.value)
        else
            mainViewModel.updateMacro(_macro.value.id, _macro.value)
    }

}

class MacroPropertiesViewModelFactory(
    private val mainViewModel: MainViewModel
) : ViewModelProvider.Factory {
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(MacroPropertiesViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return MacroPropertiesViewModel(mainViewModel) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}