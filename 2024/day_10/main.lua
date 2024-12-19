---@diagnostic disable: param-type-mismatch

local BOLD = "\27[1m"
local RESET = "\27[0m"

local START = 0
local END = 9

function Set(list)
  local set = {}
  for _, l in ipairs(list) do
    local key = tostring(l.x) .. "," .. tostring(l.y)
    set[key] = true
  end
  return set
end

function SizeOf(dict)
  local count = 0
  for _ in pairs(dict) do
    count = count + 1
  end
  return count
end

local function printBoard(board, limit)
  limit = limit or 3
  print(BOLD .. "x", "y", "height" .. RESET)
  for y, row in ipairs(board) do
    for x, height in ipairs(row) do
      limit = limit - 1
      if limit <= 0 then
        break
      end
      print(y, x, height)
    end
  end
end
local function loadBoard(filename)
  local file = io.open(filename, "r")
  if not file then
    print("File not found")
    return {}
  end

  local content = file:read(1)
  local y = 1
  Board = { {} }
  while content do
    if content == "\n" then
      y = y + 1
      Board[y] = {}
    else
      table.insert(Board[y], tonumber(content))
    end
    content = file:read(1)
  end
  table.remove(Board, #Board)
end

local function getNeighbors(x, y)
  local neighbors = {}
  local directions = {
    { x = 0,  y = -1 },
    { x = 0,  y = 1 },
    { x = -1, y = 0 },
    { x = 1,  y = 0 },
  }
  for _, direction in ipairs(directions) do
    local neighborX = x + direction.x
    local neighborY = y + direction.y
    if neighborX > 0 and neighborX <= #Board[1] and neighborY > 0 and neighborY <= #Board then
      table.insert(neighbors, { x = neighborX, y = neighborY, height = Board[neighborY][neighborX] })
    end
  end
  -- print("found neighbors: ", #neighbors)
  return neighbors
end

local function goUpHill(position, visited)
  -- print(position.x, position.y, position.height, "Go Up hill")
  if position.height == END then
    -- print("Found the end at: ", position.x, position.y)
    table.insert(visited, position)
    return
  end

  for _, neighbor in ipairs(getNeighbors(position.x, position.y)) do
    if neighbor.height - position.height == 1 then
      goUpHill(neighbor, visited)
    end
  end
end

local function runTask(calculateScore)
  loadBoard("input.txt")
  if not Board then
    print("Error loading board")
    return
  end

  local trailheadScore = 0
  for y = 1, #Board do
    for x, height in ipairs(Board[y]) do
      if height == START then
        -- print("\n" .. BOLD .. "x", "y", "height" .. RESET)
        local visited = {}
        goUpHill({ x = x, y = y, height = height }, visited)
        local newScores = calculateScore(visited)
        trailheadScore = trailheadScore + newScores
      end
    end
  end
  print("Trailhead score: ", trailheadScore)
end

Board = {}

print("Task 1:")
runTask(function(arr) return SizeOf(Set(arr)) end)

print("\nTask 2:")
runTask(function(arr) return SizeOf(arr) end)

