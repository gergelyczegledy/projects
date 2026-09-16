package hu.bme.aut.android.macroapp.presentation.macroproperties

import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material.icons.filled.Done
import androidx.compose.material3.*
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Alignment.Companion.CenterVertically
import androidx.compose.ui.text.input.KeyboardType
import hu.bme.aut.android.macroapp.domain.data.Direction
import hu.bme.aut.android.macroapp.domain.data.Action.Click
import hu.bme.aut.android.macroapp.domain.data.Action.Swipe
import hu.bme.aut.android.macroapp.domain.data.Action.Wait
import kotlinx.coroutines.launch
import java.util.UUID

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MacroPropertiesScreen(
    navController: NavHostController,
    macroPropertiesViewModel: MacroPropertiesViewModel
) {
    val macro by macroPropertiesViewModel.macro.collectAsState()

    var editableActions by remember(macro.id, macro.actions) {
        mutableStateOf(macro.actions.toMutableList())
    }

    Scaffold(
        modifier = Modifier.fillMaxSize(),
        floatingActionButton = {
            LargeFloatingActionButton(
                onClick = {
                    macroPropertiesViewModel.setMacro(
                        macro.copy(actions = editableActions.toList())
                    )
                    macroPropertiesViewModel.updateOrAddMacro()
                    navController.navigate("mainScreen")
                }
            ) {
                Icon(
                    Icons.Default.Done,
                    contentDescription = "Add new action",
                    modifier = Modifier.size(size = 50.dp)
                )
            }
        }
    ) { padding ->
        LazyColumn(
            modifier = Modifier.padding(16.dp).fillMaxWidth()
        ) {
            items(
                count = editableActions.size,
                key = { index -> UUID.randomUUID() } //szebb megoldas kene ide mert ez csak azert van itt hogy a torlesnel azonnal a jo lista latszodjon a kepernyon.
            ) { index ->
                var direction = (editableActions[index] as? Swipe)?.direction ?: Direction.UP
                var x = (editableActions[index] as? Click)?.x?.toString() ?: "0"
                var y = (editableActions[index] as? Click)?.y?.toString() ?: "0"
                var waitMs = (editableActions[index] as? Wait)?.milliseconds?.toString() ?: "0"

                var actionTypeExpanded by remember { mutableStateOf(false) }
                var actionType = editableActions[index]::class.simpleName ?: ""

                Row(
                    verticalAlignment = CenterVertically,
                    modifier = Modifier.fillMaxWidth()
                ) {
                    Box(
                        modifier = Modifier.weight(9f),
                    ){
                        Row(){
                            ExposedDropdownMenuBox(
                                expanded = actionTypeExpanded,
                                onExpandedChange = { actionTypeExpanded = !actionTypeExpanded }
                            ) {
                                OutlinedTextField(
                                    value = actionType,
                                    onValueChange = {},
                                    readOnly = true,
                                    label = { Text("Type") },
                                    trailingIcon = {
                                        ExposedDropdownMenuDefaults.TrailingIcon(expanded = actionTypeExpanded)
                                    },
                                    modifier = Modifier.menuAnchor().width(120.dp)
                                )
                                ExposedDropdownMenu(
                                    expanded = actionTypeExpanded,
                                    onDismissRequest = { actionTypeExpanded = false }
                                ) {
                                    listOf("Swipe", "Click", "Wait").forEach { type ->
                                        DropdownMenuItem(
                                            text = { Text(type) },
                                            onClick = {
                                                when (type) {
                                                    "Swipe" -> {
                                                        editableActions[index] = Swipe(direction, editableActions[index].id)
                                                    }
                                                    "Click" -> {
                                                        editableActions[index] = Click(x.toInt(), y.toInt(), editableActions[index].id)
                                                    }
                                                    "Wait" -> {
                                                        editableActions[index] = Wait(waitMs.toInt(), editableActions[index].id)
                                                    }
                                                }
                                                actionType = type
                                                actionTypeExpanded = false
                                            }
                                        )
                                    }
                                }
                            }
                            Spacer(modifier = Modifier.width(1.dp))
                            when (editableActions[index]) {
                                is Swipe -> {
                                    SwipeBox(
                                        swipe = editableActions[index] as Swipe,
                                        direction = direction,
                                        onSwipeChange = { editableActions[index] = it }
                                    )
                                }
                                is Click -> {
                                    ClickBox(
                                        click = editableActions[index] as Click,
                                        onCLickChange = { editableActions[index] = it },
                                        x = x,
                                        y = y
                                    )
                                }
                                is Wait -> {
                                    WaitBox(
                                        wait = editableActions[index] as Wait,
                                        waitMs = waitMs,
                                        onWaitChange = { editableActions[index] = it }
                                    )
                                }
                            }
                        }
                    }
                    Spacer(modifier = Modifier.width(1.dp))
                    IconButton(
                        onClick = {
                            val newList = editableActions.toMutableList().apply {
                                removeAt(index)
                            }
                            editableActions = newList
                            macroPropertiesViewModel.setMacro(
                                macro.copy(actions = editableActions)
                            )
                        },
                        modifier = Modifier
                            .weight(1f)
                            .height(OutlinedTextFieldDefaults.MinHeight)
                            .align(Alignment.Bottom)
                            .border(
                                width = OutlinedTextFieldDefaults.UnfocusedBorderThickness,
                                color = MaterialTheme.colorScheme.outline,
                                shape = OutlinedTextFieldDefaults.shape
                            )
                            .padding(0.dp)
                    ) {
                        Icon(
                            imageVector = Icons.Filled.Delete,
                            contentDescription = "Delete action",
                            tint = MaterialTheme.colorScheme.onSurfaceVariant
                        )
                    }
                }
                Spacer(modifier = Modifier.height(16.dp))
            }
            item {
                Button(
                    onClick = {
                        val newId = (editableActions.maxOfOrNull { it.id } ?: -1) + 1
                        val newList = editableActions.toMutableList().apply {
                            add(Swipe(direction = Direction.UP, id = newId))
                        }
                        editableActions = newList
                    },
                    contentPadding = PaddingValues(
                        start = 20.dp,
                        top = 10.dp,
                        end = 20.dp,
                        bottom = 10.dp
                    )
                ) {
                    Icon(
                        imageVector = Icons.Filled.Add,
                        contentDescription = "Add new action",
                        modifier = Modifier.size(size = 30.dp)
                    )
                }
                Spacer(modifier = Modifier.height(40.dp))
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SwipeBox(
    swipe: Swipe,
    direction: Direction,
    onSwipeChange: (Swipe) -> Unit
){
    var direction by remember { mutableStateOf(direction) }
    var swipe by remember { mutableStateOf(swipe) }
    var directionExpanded by remember { mutableStateOf(false) }

    ExposedDropdownMenuBox(
        expanded = directionExpanded,
        onExpandedChange = { directionExpanded = !directionExpanded },
        modifier = Modifier.fillMaxWidth()
    ) {
        OutlinedTextField(
            value = direction.name,
            onValueChange = {},
            readOnly = true,
            label = { Text("Direction") },
            trailingIcon = {
                ExposedDropdownMenuDefaults.TrailingIcon(expanded = directionExpanded)
            },
            modifier = Modifier.menuAnchor(type = MenuAnchorType.PrimaryEditable).fillMaxWidth()
        )
        DropdownMenu(
            expanded = directionExpanded,
            onDismissRequest = { directionExpanded = false }
        ) {
            Direction.entries.forEach { dir ->
                DropdownMenuItem(
                    text = { Text(dir.name) },
                    onClick = {
                        direction = dir
                        directionExpanded = false
                        swipe = Swipe(direction, swipe.id)
                        onSwipeChange(swipe)
                    }
                )
            }
        }
    }
}

@Composable
fun ClickBox(
    click: Click,
    onCLickChange: (Click) -> Unit,
    x: String,
    y: String
) {
    var click by remember { mutableStateOf(click) }
    var x by remember { mutableStateOf(x) }
    var y by remember { mutableStateOf(y) }

    Row(
        verticalAlignment = CenterVertically,
    ) {
        OutlinedTextField(
            value = x,
            onValueChange = { newX ->
                x = newX
                onCLickChange(Click(newX.toIntOrNull() ?: 0, y.toIntOrNull() ?: 0, click.id))
            },
            label = { Text("X") },
            modifier = Modifier.weight(1f),
            keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number)

        )
        Spacer(modifier = Modifier.width(1.dp))
        OutlinedTextField(
            value = y,
            onValueChange = { newY ->
                y = newY
                onCLickChange(Click(x.toIntOrNull() ?: 0, newY.toIntOrNull() ?: 0, click.id))
            },
            label = { Text("Y") },
            modifier = Modifier.weight(1f),
            keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number)
        )
    }
}

@Composable
fun WaitBox(
    wait: Wait,
    waitMs: String,
    onWaitChange: (Wait) -> Unit
){
    var wait by remember { mutableStateOf(wait) }
    var waitMs by remember { mutableStateOf(waitMs.toString()) }

    OutlinedTextField(
        value = waitMs,
        onValueChange = {
            waitMs = it
            wait = Wait(it.toIntOrNull() ?: 1000, wait.id)
            onWaitChange(wait)
        },
        label = { Text("Milliseconds") },
        keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
        modifier = Modifier.fillMaxWidth()
    )
}