package hu.bme.aut.android.macroapp

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import dagger.hilt.android.AndroidEntryPoint
import hu.bme.aut.android.macroapp.presentation.macroproperties.MacroPropertiesViewModel
import hu.bme.aut.android.macroapp.presentation.macroproperties.MacroPropertiesViewModelFactory
import hu.bme.aut.android.macroapp.presentation.main.MainViewModel
import hu.bme.aut.android.macroapp.presentation.scaffold.ScaffoldScreen
import hu.bme.aut.android.macroapp.presentation.scaffold.ScaffoldViewModel
import kotlin.getValue

@AndroidEntryPoint
class MainActivity : ComponentActivity() {
    private val scaffoldViewModel: ScaffoldViewModel by viewModels()
    private val mainViewModel: MainViewModel by viewModels()
    private val macroPropertiesViewModel: MacroPropertiesViewModel by viewModels {
        MacroPropertiesViewModelFactory(mainViewModel)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            ScaffoldScreen(
                scaffoldViewModel = scaffoldViewModel,
                mainViewModel = mainViewModel,
                macroPropertiesViewModel = macroPropertiesViewModel
            )
        }
    }
}