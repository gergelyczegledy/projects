package hu.bme.aut.android.macroapp.presentation.ui.navigation

import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import hu.bme.aut.android.macroapp.domain.data.Macro
import hu.bme.aut.android.macroapp.presentation.macroproperties.MacroPropertiesScreen
import hu.bme.aut.android.macroapp.presentation.macroproperties.MacroPropertiesViewModel
import hu.bme.aut.android.macroapp.presentation.main.MainScreen
import hu.bme.aut.android.macroapp.presentation.main.MainViewModel
import hu.bme.aut.android.macroapp.presentation.scaffold.ScaffoldViewModel

@Composable
fun NavGraph(
    modifier: Modifier = Modifier,
    navController: NavHostController = rememberNavController(),
    startDestination: String = "mainScreen",
    scaffoldViewModel: ScaffoldViewModel,
    mainViewModel: MainViewModel,
    macroPropertiesViewModel: MacroPropertiesViewModel
) {
    NavHost(
        navController = navController,
        startDestination = startDestination,
        modifier = modifier
    ) {
        composable("mainScreen"){
            scaffoldViewModel.updateTitle("Macro app", false)
            MainScreen(navController = navController, viewModel = mainViewModel)
        }

        composable(
            "macroPropertiesScreen/{macroId}/{macroName}",
            arguments = listOf(
                navArgument("macroId") { type = NavType.IntType },
                navArgument("macroName") { type = NavType.StringType }
            )
        ){
            val macroId = it.arguments?.getInt("macroId") ?: -1
            val macroName = it.arguments?.getString("macroName") ?: ""
            val macro: Macro
            if (macroId == -1){
                macro = Macro(name = macroName)
                scaffoldViewModel.updateTitle(macroName, true)
            } else{
                macro = mainViewModel.macros.value.find { it.id == macroId }!!
                scaffoldViewModel.updateTitle(macro.name, true)
            }
            macroPropertiesViewModel.setMacro(macro)

            scaffoldViewModel.setNavigationLambda { navController.navigate("mainScreen") }
            MacroPropertiesScreen(
                navController = navController,
                macroPropertiesViewModel = macroPropertiesViewModel
            )
        }
    }
}