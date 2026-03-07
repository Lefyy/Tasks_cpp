#pragma once

#include<vector>
#include<unordered_map>
#include<optional>

#include "../domain/project.h"
#include "repository.h"

class ProjectRepository : public Repository {
public:
    virtual ~ProjectRepository() = default;

    virtual void add(const Project& project) = 0;
    virtual bool remove(int projectId) = 0;
    virtual std::optional<Project> findById(int projectId) const = 0;
    virtual std::vector<Project> findAll() const = 0;
    virtual std::vector<Project> findActive() const = 0;
    virtual std::vector<Project> findDraft() const = 0;
    virtual bool update(const Project& project) = 0;
};

class InMemoryProjectRepository final : public ProjectRepository {
public:
    void add(const Project& project) override;
    bool remove(int projectId) override;
    std::optional<Project> findById(int projectId) const override;
    std::vector<Project> findAll() const override;
    std::vector<Project> findActive() const override;
    std::vector<Project> findDraft() const override;
    bool update(const Project& project) override;

private:
    std::unordered_map<int, Project> projects_;
};
