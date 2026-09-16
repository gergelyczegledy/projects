package hu.bme.aut.android.macroapp.presentation.scaffold

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.sp
import androidx.compose.runtime.livedata.observeAsState
import androidx.navigation.compose.rememberNavController
import hu.bme.aut.android.macroapp.presentation.macroproperties.MacroPropertiesViewModel
import hu.bme.aut.android.macroapp.presentation.main.MainViewModel
import hu.bme.aut.android.macroapp.presentation.ui.navigation.NavGraph

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ScaffoldScreen(
    scaffoldViewModel: ScaffoldViewModel,
    mainViewModel: MainViewModel,
    macroPropertiesViewModel: MacroPropertiesViewModel
){
    val title by scaffoldViewModel.title.observeAsState("")
    val navigationLambda by scaffoldViewModel.navigate.observeAsState()
    val showBackArrow by scaffoldViewModel.showBackArrow.observeAsState(false)

    Scaffold(
        topBar = {
            TopAppBar(
                navigationIcon = {
                    if (showBackArrow)
                        IconButton(
                            onClick = { navigationLambda?.invoke() },
                            content = {
                                Icon(
                                    imageVector = Icons.AutoMirrored.Filled.ArrowBack,
                                    contentDescription = "Back"
                                )
                            }
                        )
                },
                title = {
                    Text(
                        text = title,
                        fontSize = 30.sp
                    )
                }
            )
        },
        modifier = Modifier.fillMaxSize()
    ) { innerPadding ->
        val navController = rememberNavController()

        NavGraph(
            modifier = Modifier.padding(innerPadding),
            navController = navController,
            scaffoldViewModel = scaffoldViewModel,
            mainViewModel = mainViewModel,
            macroPropertiesViewModel = macroPropertiesViewModel
        )
    }
}