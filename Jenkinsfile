pipeline {
    agent {
        docker 'cuberite/docker-ci/minimal:latest'
    }
    stages {
        stage("CheckBasicStyle") {
            steps {
                sh 'find . -name *.cpp -or -name *.h > AllFiles.lst'
                sh 'lua CheckBasicStyle.lua'
                sh 'cd Bindings && lua CheckBindingsDependencies.lua'
            }
        }
        stage("clang-tidy") {
            steps {
                sh './clang-tidy.sh -j 2'
            }
        }
    }
}
