
convertColTypes = function(frame) {
  frame = df
  res = unlist(lapply(frame, class))
  res = lapply(res, FUN = function(x) {
    if ((x == "numeric") || (x == "integer")) {
      return("num")
    } else if (x == "factor") {
      return("categ")
    }
  })
  res = unlist(res)
  res = unname(res)
  return(res)
}

convertCategEncodings = function(data) {
  types = convertColTypes(data)
  categ_ix = which(types == "categ")
  encodings = lapply(categ_ix, FUN = function(j) {
    levels = levels(data[ , j])
    encodings = unique(as.double(data[ , j])) - 1
    l = list(levels, encodings)
    return(l)
  })
  names(encodings) = categ_ix - 1
  return(encodings)
}

xtree = function(data, target, params) {
  coltypes = convertColTypes(data)
  # categ_encodings = convertCategEncodings(data)
  # for (j in which(coltypes == "categ")) {
  #   data[ , j] = as.integer(data[ , j])
  # }
  tree = XTree$new(data, target, coltypes, params)
  return(tree)
}

convertToParty = function(tree, data) {
  library(partykit)
  strct = tree$getTreeStructure()
  strct = data.frame(apply(strct, 2, as.character), stringsAsFactors = FALSE)
  strct$party_ID = as.integer(0:(nrow(strct) - 1))
  depth = tree$depth
  
  node_list = list()
  while (depth >= 0) {
    id_vec = which(nchar(strct$ID) == depth + 1)
    parent_nodes = strct[id_vec, ]
    parent_nodes = parent_nodes[parent_nodes$is_leaf == 0, ]
    leaf_nodes = strct[id_vec, ]
    leaf_nodes = leaf_nodes[leaf_nodes$is_leaf == 1, ]
    
    leafnode_list = list()
    if (nrow(leaf_nodes) > 0) {
      for (i in 1:nrow(leaf_nodes)) {
        party_id = leaf_nodes[i, "party_ID"]
        n = partynode(id = party_id, info = list("ID" = leaf_nodes[i, "ID"]))
        leafnode_list = append(leafnode_list, list(n))
      }
      node_list = append(node_list, leafnode_list)
    }
    if (nrow(parent_nodes) > 0) {
      parentnode_list = list()
      for (i in 1:nrow(parent_nodes)) {
        parent_id = parent_nodes[i, "ID"]
        party_id = parent_nodes[i, "party_ID"]
        # childnode_candidate_id_vec = which(nchar(strct$ID) == depth + 2)
        child_nodes = list()
        for (j in 1:length(node_list)) {
          x = node_list[[j]]
          x_id_splitstring = unlist(strsplit(as.character(x$info$ID), ""))
          x_id_pasted_prefix = paste0(x_id_splitstring[-length(x_id_splitstring)], collapse = "")
          if (parent_id == x_id_pasted_prefix) {
            child_nodes = append(child_nodes, list(x))
          } else {}
        }
        child_postfixes = lapply(child_nodes, FUN = function(node) {
          id = node$info$ID
          id = unlist(strsplit(id, ""))
          postfix = as.integer(id[length(id)])
        })
        child_nodes_sorted = lapply(1:length(child_nodes), FUN = function(x) NULL)
        for (l in 1:length(child_postfixes)) {
          postfix = child_postfixes[[l]]
          child_nodes_sorted[postfix + 1] = child_nodes[l] 
        }
        split_feature = as.integer(strct[strct$ID == parent_id, "feature"])
        split_type = strct[strct$ID == parent_id, "type"]
        if (split_type == "num") {
          split_values = as.numeric(unlist(strsplit(strct[strct$ID == parent_id, "values"], ",")))
          sp = partysplit(varid = split_feature, breaks = split_values)
          n = partynode(id = party_id, split = sp, info = list("ID" = parent_id), kids = child_nodes_sorted)
          parentnode_list = append(parentnode_list, list(n))
        } else if (split_type == "categ") {
          split_ix = unlist(strsplit(strct[strct$ID == parent_id, "levels"], split = "|", fixed = TRUE))
          split_ix = gsub("[{}]", "", split_ix)
          split_ix = strsplit(split_ix, split = ",")
          n_levels_per_subset = unlist(lapply(split_ix, FUN = function(x) length(x)))
          split_ix = lapply(1:length(split_ix), FUN = function(s) {
            x = unlist(split_ix[s])
            x = rep(n_levels_per_subset[s], length(x))
            return(x)
          })
          split_ix = unlist(split_ix)
          sp = partysplit(varid = split_feature, index = split_ix)
          n = partynode(id = party_id, split = sp, info = list("ID" = parent_id), kids = child_nodes_sorted)
          parentnode_list = append(parentnode_list, list(n))
        }
      }
      node_list = append(node_list, parentnode_list)
    }
    depth = depth - 1
  }
  root = node_list[[length(node_list)]]
  party_obj = party(root, data)
  return(party_obj)
}


createPartyID = function(tree) {
  strct = tree$getTreeStructure()
  strct = data.frame(apply(strct, 2, as.character), stringsAsFactors = FALSE)
  
  n_nodes = nrow(strct)
  strct$party_id = NA
  current_id = "0"
  num_cnt = 2
  strct[which(strct$ID == 0), "party_id"] = 1
  while (num_cnt <= n_nodes) {
    row = which(strct$ID == current_id)
    print(row)
    print(current_id)
    if (is.na(strct[row, "party_id"])) {
      strct[row, "party_id"] = num_cnt
      num_cnt = num_cnt + 1
    } else {}
      lower_tier_candidates = strct[which(nchar(strct$ID) == nchar(current_id) + 1), ]
      lower_tier_ids = strsplit(as.character(lower_tier_candidates$ID), "")
      lower_tier_children = unlist(lapply(lower_tier_ids, FUN = function(x) {
        paste0(x[-length(x)], collapse = "") == current_id
      }))
      lower_tier_candidates = lower_tier_candidates[lower_tier_children, ]
    if (nrow(lower_tier_candidates) > 0) {
      # proceed to lower tier
      splitted_id_vec = strsplit(lower_tier_candidates[ , ]$ID, "")
      postfix_vec = unlist(lapply(splitted_id_vec, FUN = function(x) {
        postfix = as.integer(x[length(x)])
      }))
      leftmost_ix = 1
      while (!is.na(lower_tier_candidates[leftmost_ix, "party_id"])) {
        leftmost_ix = leftmost_ix + 1
      }
      if (leftmost_ix <= length(postfix_vec)) {
        # proceed to leftmost child without enumeration
        current_id = lower_tier_candidates[leftmost_ix, "ID"]
      } else {
        # else backtrack to upper tier
        current_id = unlist(strsplit(current_id, ""))
        current_id = current_id[-length(current_id)]
        current_id = paste0(current_id, collapse = "")
      }
    } else {
      # backtrack to upper tier
      current_id = unlist(strsplit(current_id, ""))
      current_id = current_id[-length(current_id)]
      current_id = paste0(current_id, collapse = "")
    }
  }
}
  