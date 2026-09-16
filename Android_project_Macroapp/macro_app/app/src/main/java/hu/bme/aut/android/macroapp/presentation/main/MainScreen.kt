package hu.bme.aut.android.macroapp.presentation.main

import androidx.compose.foundation.ExperimentalFoundationApi
import androidx.compose.foundation.combinedClickable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.LargeFloatingActionButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.window.DialogProperties
import androidx.navigation.NavHostController
import hu.bme.aut.android.macroapp.domain.data.Action
import java.net.URLEncoder
import java.nio.charset.StandardCharsets

@OptIn(ExperimentalMaterial3Api::class, ExperimentalFoundationApi::class)
@Composable
fun MainScreen(
    viewModel: MainViewModel,
    navController: NavHostController
) {
    val macros by viewModel.macros.collectAsState()

    var showDialog by remember { mutableStateOf(false) }
    var newMacroName by remember { mutableStateOf("") }

    val context = LocalContext.current

    Scaffold(
        modifier = Modifier.fillMaxSize(),
        floatingActionButton = {
            LargeFloatingActionButton(
                onClick = {
                    newMacroName = ""
                    showDialog = true
                }
            ) {
                Icon(
                    Icons.Default.Add,
                    contentDescription = "Add new action",
                    modifier = Modifier.size(size = 50.dp)
                )
            }
        }
    ) { padding ->
        if (showDialog) {
            AlertDialog(
                onDismissRequest = {
                    showDialog = false
                    newMacroName = ""
                },
                title = { Text("New Macro") },
                text = {
                    Column {
                        OutlinedTextField(
                            value = newMacroName,
                            onValueChange = { newMacroName = it },
                            label = { Text("Macro Name") },
                            singleLine = true,
                            modifier = Modifier.fillMaxWidth()
                        )
                    }
                },
                confirmButton = {
                    TextButton(
                        onClick = {
                            if (newMacroName.isNotBlank()) {
                                showDialog = false
                                val encodedMacroName = URLEncoder.encode(newMacroName, StandardCharsets.UTF_8.toString())
                                navController.navigate("macroPropertiesScreen/-1/$encodedMacroName")
                                newMacroName = ""
                            } else {

                            }
                        }
                    ) {
                        Text("Create")
                    }
                },
                dismissButton = {
                    TextButton(
                        onClick = {
                            showDialog = false
                            newMacroName = ""
                        }
                    ) {
                        Text("Cancel")
                    }
                },
                properties = DialogProperties(dismissOnClickOutside = true, dismissOnBackPress = true)
            )
        }

        LazyColumn(modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)) {
            items(macros.size) { index ->
                val macro = macros[index]
                Card(
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(vertical = 4.dp)
                        .combinedClickable(
                            onClick = {
                                navController.navigate("macroPropertiesScreen/${macro.id}/\"\"")
                            }
                        ),
                    elevation = CardDefaults.cardElevation(defaultElevation = 4.dp)
                ) {
                    Row(
                        modifier = Modifier.fillMaxWidth(),
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        Column(
                            modifier = Modifier
                                .weight(8f)
                                .padding(8.dp)
                        ){
                            Text(
                                text = macro.name,
                                style = MaterialTheme.typography.titleMedium,
                            )
                            macro.actions.forEachIndexed {index, action ->
                                if (index >= 5){
                                    Text(text = "...", style = MaterialTheme.typography.bodySmall)
                                    return@Column
                                }
                                when (action) {
                                    is Action.Swipe -> Text(text = "Swipe ${action.direction}", style = MaterialTheme.typography.bodyMedium)
                                    is Action.Click -> Text(text = "Click at (${action.x}, ${action.y})", style = MaterialTheme.typography.bodyMedium)
                                    is Action.Wait -> Text(text = "Wait " + action.milliseconds +" ms")
                                }
                            }
                        }
                        IconButton(
                            onClick = { viewModel.runMacro(macro, context) },
                            modifier = Modifier
                                .weight(1f)
                        ) {
                            Icon(
                                imageVector = Icons.Filled.PlayArrow,
                                contentDescription = "Run macro",
                                tint = MaterialTheme.colorScheme.onSurfaceVariant
                            )
                        }
                        IconButton(
                            onClick = { viewModel.deleteMacro(macro.id) },
                            modifier = Modifier.weight(1f)

                        ) {
                            Icon(
                                imageVector = Icons.Filled.Delete,
                                contentDescription = "Delete macro",
                                tint = MaterialTheme.colorScheme.onSurfaceVariant
                            )
                        }
                    }
                }
            }
        }
    }
}