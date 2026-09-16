package hu.bme.aut.android.macroapp.presentation.scaffold

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class ScaffoldViewModel : ViewModel(){
    private val _title = MutableLiveData("Macro app")
    val title: LiveData<String> = _title

    private val _navigate = MutableLiveData<() -> Unit>()
    val navigate: LiveData<() -> Unit> = _navigate

    private val _showBackArrow = MutableLiveData(false)
    val showBackArrow: LiveData<Boolean> = _showBackArrow

    fun updateTitle(newTitle: String, showBackArrow: Boolean) {
        _title.value = newTitle
        _showBackArrow.value = showBackArrow
    }

    fun setNavigationLambda(lambda: () -> Unit) {
        _navigate.value = lambda
    }
}